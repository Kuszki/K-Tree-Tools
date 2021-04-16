/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  {description}                                                          *
 *  Copyright (C) 2020  Łukasz "Kuszki" Dróżdż  lukasz.kuszki@gmail.com    *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "threadworker.hpp"

ThreadWorker::ThreadWorker(QObject *parent)
	: QObject(parent)
{}

ThreadWorker::~ThreadWorker(void) {}

bool ThreadWorker::isCanceled(void) const
{
	QMutexLocker Locker(&Mutex);

	return Job && Kill;
}

bool ThreadWorker::isStartable(void) const
{
	QMutexLocker Locker(&Mutex);

	return !Job && !Kill;
}

Common::NODELIST ThreadWorker::getNodes(const QString& path, QSharedPointer<Common::NODE> parent) const
{
	if (!QFileInfo(path).isDir()) return Common::NODELIST();

	const auto filter = QDir::Files | QDir::Dirs |
					QDir::Hidden | QDir::System |
					QDir::NoDotAndDotDot;
	const int level = parent ? parent->level + 1 : 0;

	Common::NODELIST list, childs;

	if (parent.isNull())
	{
		parent = Common::NODEPTR(new Common::NODE({ QFileInfo(path), -1, nullptr }));
	}

	for (const auto& e : QDir(path).entryInfoList(filter))
	{
		list.append(Common::NODEPTR(new Common::NODE({ e, level, parent })));
	}

	for (auto& d : list) if (d->info.isDir())
	{
		childs.append(getNodes(d->info.absoluteFilePath(), d));
	}

	if (level == 0) list.append(parent);
	if (!childs.isEmpty()) list.append(childs);

	return list;
}

QVariantList ThreadWorker::getTable(const QString& path, int col) const
{
	QVariantList list;

	QFile file(path);
	QTextStream stream(&file);

	if (!file.exists() || !file.open(QFile::ReadOnly | QFile::Text))
		return QVariantList();

	const auto sep = QFileInfo(file).suffix() == "csv" ? ';' : '\t';
	const auto skip = (sep == ';') ? Qt::KeepEmptyParts :
							   Qt::SkipEmptyParts;

	while (!stream.atEnd())
	{
		const auto l = stream.readLine().remove('\n').remove('\r');
		const QStringList r = l.split(sep, skip);

		if (col == -1 && !r.isEmpty()) list.append(r);
		else if (col < r.size()) list.append(r[col]);
	}

	return list;
}

QStringList ThreadWorker::validateDepth(const Common::NODELIST& nodes, int min, int max, int files)
{
	Common::NODELIST listA, listB;
	QMutex sync, syncA, syncB;
	QStringList list;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] () -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	QtConcurrent::blockingMap(nodes,
	[this, &nodes, &listA, &listB, &syncA, &syncB, &progress, min, max, files]
	(const auto& n) -> void
	{
		if (isCanceled()) return;
		bool goodFound(false);

		if (n->info.isFile()) goodFound = n->level >= files;
		else if (n->info.isDir() && min > 1 && n->level < min-1)
		{
			for (const auto& c : nodes)
				if (c->level > n->level && c->info.isDir())
				{
					auto parent = c->parent;

					while (parent && parent != n)
					{
						parent = parent->parent;
					}

					if (parent == n)
					{
						goodFound = true; break;
					}
				}
		}
		else goodFound = true;

		if (!goodFound)
		{
			syncA.lock();
			listA.append(n);
			syncA.unlock();
		}

		if (n->info.isDir() && n->level >= max)
		{
			syncB.lock();
			listB.append(n);
			syncB.unlock();
		}

		progress();
	});

	const QString msgA = tr("Not enough deep (%1) in object: %2");
	const QString msgB = tr("Too high depth (%1) in object: %2");

	for (const auto& l : listA) list.append(msgA
		.arg(l->level + (l->info.isDir() ? 1 : 0))
		.arg(l->info.absoluteFilePath()));

	for (const auto& l : listB) list.append(msgB
		.arg(l->level + (l->info.isDir() ? 1 : 0))
		.arg(l->info.absoluteFilePath()));

	return list;
}

QStringList ThreadWorker::validateEmpty(const Common::NODELIST& nodes, int level, int action, int obj)
{
	Common::NODESET listA;
	QMutex sync, syncA;
	QStringList list;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] () -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	QtConcurrent::blockingMap(nodes,
	[this, &nodes, &listA, &syncA, &progress, level, action, obj]
	(const auto& n) -> void
	{
		if (isCanceled()) return;
		bool foundChild(false);

		if ((action == 0 && n->info.isDir()) &&
		    ((level == -1) || (level - 1 == n->level)))
		{
			for (const auto& d : nodes)
			{
				const bool fok = d->info.isFile() && (obj == 0 || obj == 2);
				const bool dok = d->info.isDir() && (obj == 1 || obj == 2);

				if (fok || dok)
				{
					if (n == d->parent)
					{
						foundChild = true; break;
					}
				}
			}
		}
		else foundChild = true;

		if (!foundChild || (
			    (action == 1 && n->info.isFile()) &&
			    ((level == -1) || (level == n->level))))
		{
			syncA.lock();
			listA.insert(n);
			syncA.unlock();
		}

		progress();
	});

	const QString msgA = action == 1 ?
		tr("Forbiden object in level (%1): %2") :
		tr("Forbiden empty level (%1): %2");

	for (const auto& l : listA) list.append(msgA
		.arg(l->level + (l->info.isDir() ? 1 : 0))
		.arg(l->info.absoluteFilePath()));

	return list;
}

QStringList ThreadWorker::validateImages(const Common::NODELIST& nodes, int level, int dpi, int qual, const QStringList& filter)
{
	QMutex sync, syncl;
	QStringList list;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] () -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	QtConcurrent::blockingMap(nodes,
	[this, &list, &syncl, &progress, &filter, level, dpi, qual]
	(const auto& n) -> void
	{
		if (isCanceled()) return;

		if (n->info.isFile() && (level == -1 || n->level == level) &&
		    filter.contains(n->info.suffix(), Qt::CaseInsensitive))
		{
			const QString path = n->info.absoluteFilePath();

			const QString ferror = tr("Unable to process image (%2): %1").arg(path);
			const QString qerror = tr("Wrong quality (%2%) in image: %1").arg(path);
			const QString perror = tr("Unable to process image (%2) (page %3): %1").arg(path);
			const QString derror = tr("Wrong resolution (%2 DPI) in image (page %3): %1").arg(path);
			const QString ierror = tr("Unable to process %2 from %3: %1").arg(path);

			QImageReader file(path);
			QStringList local;

			if (!file.canRead()) local.append(ferror.arg(file.errorString()));
			else
			{
				if (file.quality() != -1 && file.quality() < qual)
					local.append(qerror.arg(file.quality()));

				const auto total = file.imageCount();
				auto count = total;

				int i(0); do
				{
					const auto img = file.read(); ++i;

					if (img.isNull()) local.append(perror
											 .arg(file.errorString())
											 .arg(i));
					else
					{
						const int res = Common::getDPI(img);

						if (qAbs(res - dpi) >= 5) local.append(
									derror.arg(res).arg(i));

						--count;
					}
				}
				while (file.jumpToNextImage());

				if (count && (total != -1)) local.append(ierror.arg(count)
							   .arg(tr("%n image page(s)", nullptr, total)));
			}

			if (!local.isEmpty())
			{
				syncl.lock();
				list.append(local);
				syncl.unlock();
			}
		}

		progress();
	});

	return list;
}

QStringList ThreadWorker::validateItems(const Common::NODELIST& nodes, int level, int action, int col, const QString& path)
{
	QSet<QString> check, found;
	QMutex sync, syncl;
	QStringList list;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] () -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	for (const auto& i : getTable(path, col))
	{
		check.insert(i.toString());
	}

	if (check.isEmpty()) return
	{
		tr("Unable to load list from file: %1").arg(path)
	};

	if (action == 0 || action == 2) QtConcurrent::blockingMap(nodes,
	[this, &nodes, &check, &list, &syncl, &progress, level, action]
	(const auto& n) -> void
	{
		if (isCanceled()) return;

		if (n->info.isDir() && (level == -1 || n->level == level-1))
		{
			const QString bFound = tr("Forbiden file (%2) on level (%3) in: %1")
							   .arg(n->info.absoluteFilePath());
			const QString nFound = tr("Missing file (%2) on level (%3) in: %1")
							   .arg(n->info.absoluteFilePath());
			QSet<QString> copy = check;
			QStringList local;

			for (const auto& c : nodes) if (n == c->parent)
			{
				const auto fn = c->info.fileName();

				if (action == 2 && copy.contains(fn))
				{
					local.append(bFound.arg(fn).arg(c->level));
				}
				else if (action == 0)
				{
					copy.remove(fn);
				}
			}

			if (action == 0) for (const auto& fn : copy)
			{
				local.append(nFound.arg(fn).arg(n->level+1));
			}

			if (!local.isEmpty())
			{
				syncl.lock();
				list.append(local);
				syncl.unlock();
			}
		}

		progress();
	});
	else if (action == 3 || action == 4) QtConcurrent::blockingMap(nodes,
	[this, &check, &syncl, &progress, &list, level, action]
	(const auto& n) -> void
	{
		if (isCanceled()) return; bool ok = false;

		const bool fok = (action == 3) && n->info.isFile();
		const bool dok = (action == 4) && n->info.isDir();

		const bool typeok = fok || dok;

		if (typeok && (level == -1 || n->level == level))
			for (const auto& s : check)
			{
				if (QRegExp(s).exactMatch(n->info.fileName()))
				{
					ok = true; break;
				}
			}
		else ok = true;

		if (!ok)
		{
			const QString local = tr("Object name mismatch on level (%2) in: %1")
							  .arg(n->info.absoluteFilePath())
							  .arg(n->level);

			syncl.lock();
			list.append(local);
			syncl.unlock();
		}

		progress();
	});
	else if (action == 1) QtConcurrent::blockingMap(nodes,
	[this, &check, &found, &syncl, &progress, level]
	(const auto& n) -> void
	{
		if (isCanceled()) return;

		if ((level == -1 || n->level == level) &&
		    check.contains(n->info.fileName()))
		{
			syncl.lock();
			found.insert(n->info.fileName());
			syncl.unlock();
		}

		progress();
	});

	const QString nfMsg = level == -1 ?
		tr("Object missing in any level: %1") :
		tr("Object missing in level (%1): %2").arg(level);

	if (action == 1) for (const auto& i : check-found)
	{
		list.append(nfMsg.arg(i));
	}

	return list;
}

QStringList ThreadWorker::validateTree(const Common::NODELIST& nodes, int format, int action, const QString& path)
{
	const QString sep = format ? QString() : QString('/');

	QHash<Common::NODE*, QStringList> missing;
	QMutex sync, syncfg, syncfb;
	QStringList list;

	QSet<QString> check, foundg, foundb;
	QString prefix;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] () -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	auto iter = nodes.constBegin();
	auto stp = nodes.constEnd();

	while (prefix.isEmpty() && iter != stp)
	{
		if ((*iter)->level != -1) ++iter;
		else prefix = (*iter)->info.absoluteFilePath() + '/';
	}

	for (const auto& i : getTable(path))
	{
		check.insert(prefix + i.toStringList().join(sep));
	}

	if (check.isEmpty()) return
	{
		tr("Unable to load list from file: %1").arg(path)
	};

	QtConcurrent::blockingMap(nodes,
	[this, &check, &foundg, &foundb, &syncfg, &syncfb, &progress, action]
	(const auto& n) -> void
	{
		if (isCanceled()) return;
		if (n->level < 0) return;

		const auto nm = n->info.absoluteFilePath();

		if (check.contains(nm))
		{
			syncfg.lock();
			foundg.insert(nm);
			syncfg.unlock();
		}
		else if (action == 2)
		{
			syncfb.lock();
			foundb.insert(nm);
			syncfb.unlock();
		}

		progress();
	});

	const QString missMsg = tr("Missing object: %1");
	const QString invMsg = tr("Invalid object: %1");

	if (action == 0 || action == 2)
		for (const auto& fn : check-foundg)
		{
			list.append(missMsg.arg(fn));
		}

	if (action == 1)
		for (const auto& fn : foundb)
		{
			list.append(invMsg.arg(fn));
		}

	return list;
}

QStringList ThreadWorker::validateTypes(const Common::NODELIST& nodes, int level, int action, const QStringList& filter)
{
	QMutex sync, syncl;
	QStringList list;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] () -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	const QString msg = tr("Wrong file extension (%1) in: %2");

	QtConcurrent::blockingMap(nodes,
	[this, &list, &syncl, &progress, &filter, &msg, level, action]
	(const auto& n) -> void
	{
		if (isCanceled()) return;

		if (n->info.isFile() && (level == -1 || n->level == level))
		{
			const QString path = n->info.absoluteFilePath();
			const QString ext = n->info.suffix();

			const bool match = filter.contains(ext);

			if (match && action == 1)
			{
				syncl.lock();
				list.append(msg.arg(ext).arg(path));
				syncl.unlock();
			}
			else if (!match && action == 0)
			{
				syncl.lock();
				list.append(msg.arg(ext).arg(path));
				syncl.unlock();
			}
		}

		progress();
	});

	return list;
}

QStringList ThreadWorker::countDepth(const Common::NODELIST& nodes, int level)
{
	QMutex sync, syncl;
	QStringList list;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] () -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	const std::function<QList<int> (const Common::NODEPTR paren)> countch =
	[&countch, &nodes] (const Common::NODEPTR parent) -> QList<int>
	{
		int fc(0), dc(0), dp(parent->level);

		for (const auto& n : nodes) if (n->parent == parent)
		{
			if (n->info.isFile()) ++fc;
			else
			{
				const auto r = countch(n);

				dp = qMax(dp, r.value(2, 0));

				fc += r.value(0, 0);
				dc += r.value(1, 0) + 1;
			}
		}

		return { fc, dc, dp };
	};

	QtConcurrent::blockingMap(nodes,
	[this, &list, &syncl, &progress, &countch, level]
	(const auto& n) -> void
	{
		if (isCanceled()) return;

		if (n->level == level && n->info.isDir())
		{
			const auto stat = countch(n);

			const QString msg = tr("Found %1 and %2 with maximum depth of %3 in %4")
							.arg(tr("%n file(s)", nullptr, stat.value(0, 0)))
							.arg(tr("%n directorie(s)", nullptr, stat.value(1, 0)))
							.arg(tr("%n level(s)", nullptr, stat.value(2, 0) - n->level))
							.arg(n->info.absoluteFilePath());

			syncl.lock();
			list.append(msg);
			syncl.unlock();
		}

		progress();
	});

	return list;
}

QStringList ThreadWorker::countEmpty(const Common::NODELIST& nodes, int level, int action)
{
	QHash<int, int> counter;
	QMutex sync, syncc;
	QStringList list;
	int total(0);

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] (void) -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	const auto count = [&counter, &total, &syncc] (int lvl) -> void
	{
		syncc.lock();

		if (!counter.contains(lvl)) counter[lvl] = 1;
		else counter[lvl] += 1;

		total += 1;

		syncc.unlock();
	};

	QtConcurrent::blockingMap(nodes,
	[this, &nodes, &count, &progress, level, action]
	(const auto& n) -> void
	{
		if (isCanceled()) return;
		bool foundChild(false);

		if (n->info.isDir() && ((level == -1) || (level - 1 == n->level)))
		{
			for (const auto& d : nodes)
			{
				const bool fok = d->info.isFile();
				const bool dok = d->info.isDir() && (action == 1);

				if (fok || dok)
				{
					if (n == d->parent)
					{
						foundChild = true; break;
					}
				}
			}
		}
		else foundChild = true;

		if (!foundChild) count(n->level+1);

		progress();
	});

	if (level == -1)
		for (auto i = counter.cbegin(); i != counter.cend(); ++i)
		{
			list.append(tr("%n empty directorie(s) in level %1", nullptr, i.value()).arg(i.key()));
		}
	else list.append(tr("%n empty directorie(s) in level %1", nullptr, counter.value(level)).arg(level));

	if (!total)
	{
		list.append(tr("No empty directories found"));
	}

	return list;
}

QStringList ThreadWorker::countImages(const Common::NODELIST& nodes, int level, int actions, const QStringList& filter)
{
	QMap<Common::FORMAT, int> fcount;
	QHash<int, int> dcount;
	QMutex sync, syncf, syncd, syncc, synca;
	QStringList list;
	int count(0), call(0);

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] (void) -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	const auto appendf = [&fcount, &syncf] (Common::FORMAT f) -> void
	{
		syncf.lock();

		if (!fcount.contains(f)) fcount[f] = 1;
		else fcount[f] += 1;

		syncf.unlock();
	};

	const auto appendd = [&dcount, &syncd] (int dpi) -> void
	{
		syncd.lock();

		if (!dcount.contains(dpi)) dcount[dpi] = 1;
		else dcount[dpi] += 1;

		syncd.unlock();
	};

	const auto appendc = [&count, &syncc] (void) -> void
	{
		syncc.lock(); ++count; syncc.unlock();
	};

	const auto appenda = [&call, &synca] (void) -> void
	{
		synca.lock(); ++call; synca.unlock();
	};

	QtConcurrent::blockingMap(nodes,
	[this, &progress, &filter, &appendf, &appendd, &appendc, &appenda, level]
	(const auto& n) -> void
	{
		if (isCanceled()) return;

		if (n->info.isFile() && (level == -1 || n->level == level) &&
		    filter.contains(n->info.suffix(), Qt::CaseInsensitive))
		{
			QImageReader file(n->info.absoluteFilePath());

			if (file.canRead())
			{
				auto count = file.imageCount();

				do
				{
					const auto img = file.read(); --count;

					appendf(Common::getFormat(img));
					appendd(Common::getDPI(img));
					appendc();
				}
				while (file.jumpToNextImage());
			}

			appenda();
		}

		progress();
	});

	if (!actions || (actions & 0b010))
	{
		for (auto i = fcount.cbegin(); i != fcount.cend(); ++i)
		{
			const auto name = Common::getFormat(i.key());

			list.append(tr("Found %n sheet(s) of %1 format", nullptr, i.value())
					  .arg(name));
		}
	}

	if (!actions || (actions & 0b100))
	{
		for (auto i = dcount.cbegin(); i != dcount.cend(); ++i)
		{
			list.append(tr("Found %n sheet(s) with %1 DPI", nullptr, i.value())
					  .arg(i.key()));
		}
	}

	if (!actions || (actions & 0b001))
	{
		if (count)
		{
			list.append(tr("Found total %1 %2")
					  .arg(tr("%n file(s) containing", nullptr, call))
					  .arg(tr("%n sheet(s)", nullptr, count)));
		}
	}

	if (!count)
	{
		list.append(tr("Didn't found any matching sheets"));
	}

	return list;
}

QStringList ThreadWorker::countItems(const Common::NODELIST& nodes, int level, int action, int col, const QString& path)
{
	QHash<int, int> fcount, dcount;
	QMutex sync, syncf, syncd, syncc;
	int ftotal(0), dtotal(0);
	QSet<QString> check;
	QStringList list;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] (void) -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	if (action) for (const auto& i : getTable(path, col))
	{
		check.insert(i.toString());
	}

	if (action && check.isEmpty()) return
	{
		tr("Unable to load list from file: %1").arg(path)
	};

	const auto appendf = [&fcount, &ftotal, &syncf] (int lvl) -> void
	{
		syncf.lock();

		if (!fcount.contains(lvl)) fcount[lvl] = 1;
		else fcount[lvl] += 1;

		ftotal += 1;

		syncf.unlock();
	};

	const auto appendd = [&dcount, &dtotal, &syncd] (int lvl) -> void
	{
		syncd.lock();

		if (!dcount.contains(lvl)) dcount[lvl] = 1;
		else dcount[lvl] += 1;

		dtotal += 1;

		syncd.unlock();
	};

	QtConcurrent::blockingMap(nodes,
	[this, &check, &progress, &appendf, &appendd, level, action]
	(const auto& n) -> void
	{
		if (isCanceled()) return;
		if (n->level == -1) return;

		if (level == -1 || n->level == level)
		{
			const auto nm = n->info.fileName();
			bool ok = action == 0;

			if (action == 1)
			{
				ok = check.contains(nm);
			}
			else if (action == 2)
			{
				ok = !check.contains(nm);
			}

			if (ok)
			{
				if (n->info.isFile()) appendf(n->level);
				if (n->info.isDir()) appendd(n->level);
			}
		}

		progress();
	});

	if (level == -1) for (const auto& k : (fcount.keys() + dcount.keys()).toSet())
	{
		list.append(tr("Found %1 and %2 in level %3")
				  .arg(tr("%n file(s)", nullptr, fcount.value(k, 0)))
				  .arg(tr("%n directorie(s)", nullptr, dcount.value(k, 0)))
				  .arg(k));
	}

	if (!(ftotal + dtotal))
	{
		list.append(tr("Didn't found any matching objects"));
	}
	else
	{
		list.append(tr("Found %1 and %2 in %3")
				  .arg(tr("%n file(s)", nullptr, ftotal))
				  .arg(tr("%n directorie(s)", nullptr, dtotal))
				  .arg(level == -1 ? tr("selected levels") :
								 tr("selected level")));
	}

	return list;
}

QStringList ThreadWorker::countTypes(const Common::NODELIST& nodes, int level, const QStringList& filter)
{
	QHash<int, int> fcount;
	QMutex sync, syncf, syncd, syncc;
	int ftotal(0);
	QStringList list;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] (void) -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	const auto appendf = [&fcount, &ftotal, &syncf] (int lvl) -> void
	{
		syncf.lock();

		if (!fcount.contains(lvl)) fcount[lvl] = 1;
		else fcount[lvl] += 1;

		ftotal += 1;

		syncf.unlock();
	};

	QtConcurrent::blockingMap(nodes,
	[this, &progress, &appendf, &filter, level]
	(const auto& n) -> void
	{
		if (isCanceled()) return;

		if (n->info.isFile() && (level == -1 || n->level == level) &&
		    filter.contains(n->info.suffix(), Qt::CaseInsensitive))
		{
			appendf(n->level);
		}

		progress();
	});

	if (level == -1) for (auto i = fcount.cbegin(); i != fcount.cend(); ++i)
	{
		list.append(tr("Found %1 in level %2")
				  .arg(tr("%n object(s)", nullptr, i.value()))
				  .arg(i.key()));
	}

	if (!ftotal)
	{
		list.append(tr("Didn't found any matching objects"));
	}
	else
	{
		list.append(tr("Found %1 in %2")
				  .arg(tr("%n object(s)", nullptr, ftotal))
				  .arg(level == -1 ? tr("selected levels") :
								 tr("selected level")));
	}

	return list;
}

QStringList ThreadWorker::performRemove(const Common::NODELIST& nodes, int level, int action, int col, const QString& path)
{
	QSet<QString> check;
	QMutex sync, syncl;
	QStringList list;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] () -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	for (const auto& i : getTable(path, col))
	{
		check.insert(i.toString());
	}

	if (check.isEmpty()) return
	{
		tr("Unable to load list from file: %1").arg(path)
	};

	const QString msg = tr("Removed object: %1");
	const QString err = tr("Unable to remove object: %1");

	QtConcurrent::blockingMap(nodes,
	[this, &check, &list, &syncl, &progress, &msg, &err, level, action]
	(const auto& n) -> void
	{
		if (isCanceled()) return;
		if (n->level == -1) return;

		const bool lvlok = (level == -1) || (n->level == level);
		bool rmok = false;

		switch (action)
		{
			case 0:
				rmok = check.contains(n->info.fileName());
			break;
			case 1:
				rmok = check.contains(n->info.suffix());
			break;
			case 2:
			{
				const auto nm = n->info.fileName();

				for (const auto& pattern : check)
				{
					rmok = rmok || QRegExp(pattern).exactMatch(nm);
				}
			}
		}

		if (rmok && lvlok)
		{
			const auto pt = n->info.absoluteFilePath();

			if (n->info.isDir())
			{
				rmok = QDir(pt).removeRecursively();
			}
			else if (n->info.isFile())
			{
				rmok = QFile(pt).remove();
			}
			else rmok = false;

			const auto ms = rmok ? msg : err;

			syncl.lock();
			list.append(ms.arg(pt));
			syncl.unlock();
		}

		progress();
	});

	if (list.isEmpty()) list.append(tr("No objects removed"));

	return list;
}

QStringList ThreadWorker::performCopy(const Common::NODELIST& nodes, int level, int action, int col, int format, const QString& path, const QString& dest)
{
	Common::NODELIST found;
	QSet<QString> check;
	QMutex sync, syncf;
	QStringList list;
	QString root;

	emit onJobStart(0, nodes.size()); int step(0);

	const auto progress = [&step, &sync, this] () -> void
	{
		sync.lock(); emit onJobProgress(++step); sync.unlock();
	};

	for (const auto& i : getTable(path, col))
	{
		check.insert(i.toString());
	}

	if (check.isEmpty()) return
	{
		tr("Unable to load list from file: %1").arg(path)
	};

	QtConcurrent::blockingMap(nodes,
	[this, &check, &found, &syncf, &progress, &root, level, format]
	(const auto& n) -> void
	{
		if (isCanceled()) return;
		if (n->level == -1)
		{
			syncf.lock();
			root = n->info.absoluteFilePath() + '/';
			syncf.unlock();

			return;
		}

		const bool lvlok = (level == -1) || (n->level == level);
		bool cpok = false;

		switch (format)
		{
			case 0:
				cpok = check.contains(n->info.fileName());
			break;
			case 1:
			{
				const auto nm = n->info.fileName();

				for (const auto& pattern : check)
				{
					cpok = cpok || QRegExp(pattern).exactMatch(nm);
				}
			}
		}

		if (cpok && lvlok)
		{
			syncf.lock();
			found.append(n);
			syncf.unlock();
		}

		progress();
	});

	const QString msg = tr("Copied object: %1");
	const QString err = tr("Unable to copy object: %1");

	for (const auto& i : found)
	{
		QString fpath = i->info.absoluteFilePath();
		QString fdir = i->info.absolutePath().remove(root);
		QString fname = i->info.fileName();

		bool ok(false);

		if (action == 0)
		{
			ok = Common::copyObject(fpath, dest + '/' + fname);
		}
		else if (action == 1)
		{
			const QString dst = dest + '/' + fdir +'/' + fname;

			ok = QDir(dest).mkpath(fdir);
			ok = ok && Common::copyObject(fpath, dst);
		}

		if (ok) list.append(msg.arg(fpath));
		else list.append(err.arg(fpath));
	}

	if (list.isEmpty()) list.append(tr("No objects copied"));

	return list;
}

void ThreadWorker::start(void)
{
	QMutexLocker Locker(&Mutex);

	Kill = false;
	Job = true;
}

void ThreadWorker::stop(void)
{
	QMutexLocker Locker(&Mutex);

	Kill = false;
	Job = false;
}

void ThreadWorker::startProcessList(const QString& path, const QString& logs, const QVariantList& rules)
{
	if (isStartable()) start(); else return;

	emit onJobChange(tr("Scanning directory tree"));
	emit onJobStart(0, 0); taskno = 0;

	const QString msg = tr("Job %1/%2: %3");
	QStringList list;

	const auto nodes = getNodes(path);
	const int total = rules.size();

	const auto sep = QString("=").repeated(75);

	if (!logs.isEmpty() && QDir(logs).exists())
	{
		const QString now = QDateTime::currentDateTime()
						.toString("dd.MM.yyyy hh.mm.ss");

		if (!QDir(logs).mkdir(now)) logpath = QString();
		else logpath = logs + '/' + now;
	}

	for (const auto& r : rules) if (!isCanceled())
	{
		QStringList lines;

		const QVariantMap job = r.toMap();

		const auto jname = job.value("class").toString();
		const auto jbrief = job.value("brief").toString();
		const auto jdesc = job.value("name").toString();

		const QString pname = msg.arg(++taskno)
						  .arg(total)
						  .arg(jdesc);

		emit onJobChange(pname);
		emit onJobStart(0, 0);

		emit onPartDone({ jbrief, sep });

		if (!job.value("active").toBool()) continue;
		else if (jname == "deepval")
		{
			lines = validateDepth(nodes,
				job.value("min").toInt(),
				job.value("max").toInt(),
				job.value("files").toInt());
		}
		else if (jname == "emptyval")
		{
			lines = validateEmpty(nodes,
				job.value("level").toInt(),
				job.value("action").toInt(),
				job.value("objects").toInt());
		}
		else if (jname == "imageval")
		{
			lines = validateImages(nodes,
				job.value("level").toInt(),
				job.value("dpi").toInt(),
				job.value("quality").toInt(),
				job.value("filter").toStringList());
		}
		else if (jname == "itemsval")
		{
			lines = validateItems(nodes,
				job.value("level").toInt(),
				job.value("action").toInt(),
				job.value("column").toInt()-1,
				job.value("path").toString());
		}
		else if (jname == "treeval")
		{
			lines = validateTree(nodes,
				job.value("format").toInt(),
				job.value("action").toInt(),
				job.value("path").toString());
		}
		else if (jname == "typesval")
		{
			lines = validateTypes(nodes,
				job.value("level").toInt(),
				job.value("action").toInt(),
				job.value("filter").toStringList());
		}
		else if (jname == "deeprap")
		{
			lines = countDepth(nodes,
				job.value("level").toInt());
		}
		else if (jname == "emptyrap")
		{
			lines = countEmpty(nodes,
				job.value("level").toInt(),
				job.value("action").toInt());
		}
		else if (jname == "imagerap")
		{
			lines = countImages(nodes,
				job.value("level").toInt(),
				job.value("actions").toInt(),
				job.value("filter").toStringList());
		}
		else if (jname == "itemsrap")
		{
			lines = countItems(nodes,
				job.value("level").toInt(),
				job.value("action").toInt(),
				job.value("column").toInt()-1,
				job.value("path").toString());
		}
		else if (jname == "typesrap")
		{
			lines = countTypes(nodes,
				job.value("level").toInt(),
				job.value("filter").toStringList());
		}
		else if (jname == "removejob")
		{
			lines = performRemove(nodes,
				job.value("level").toInt(),
				job.value("action").toInt(),
				job.value("column").toInt()-1,
				job.value("path").toString());
		}
		else if (jname == "copyjob")
		{
			lines = performCopy(nodes,
				job.value("level").toInt(),
				job.value("action").toInt(),
				job.value("column").toInt()-1,
				job.value("format").toInt(),
				job.value("path").toString(),
				job.value("dest").toString());
		}

		if (!lines.isEmpty())
		{
			emit onPartDone(lines);
			emit onPartDone({ sep });

			list.append(lines);
		}
	}

	emit onJobChange({ tr("Done") });
	emit onJobDone(list);

	stop();
}

void ThreadWorker::sendTerminateRequest(void)
{
	QMutexLocker Locker(&Mutex);

	if (Job) Kill = true;
}
