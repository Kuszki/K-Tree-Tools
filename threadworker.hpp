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

#ifndef THREADWORKER_HPP
#define THREADWORKER_HPP

#include <QtConcurrent>
#include <QtCore>
#include <QtGui>

#include "common.hpp"

class ThreadWorker : public QObject
{

		Q_OBJECT

	private:

		mutable QMutex Mutex;

		QString logpath;
		unsigned taskno;

		bool Kill = false;
		bool Job = false;

	public:

		explicit ThreadWorker(QObject *parent = nullptr);
		virtual ~ThreadWorker(void) override;

		bool isCanceled(void) const;
		bool isStartable(void) const;

		Common::NODELIST getNodes(const QString& path,
							 QSharedPointer<Common::NODE> parent = nullptr) const;

		QVariantList getTable(const QString& path, int col = -1) const;

		QStringList validateDepth(const Common::NODELIST& nodes,
							 int min, int max, int files);

		QStringList validateEmpty(const Common::NODELIST& nodes,
							 int level, int action, int obj);

		QStringList validateImages(const Common::NODELIST& nodes,
							  int level, int dpi, int qual,
							  const QStringList& filter);

		QStringList validateItems(const Common::NODELIST& nodes,
							 int level, int action, int col,
							 const QString& path);

		QStringList validateTree(const Common::NODELIST& nodes,
							int format, int action,
							const QString& path);

		QStringList validateTypes(const Common::NODELIST& nodes,
							 int level, int action,
							 const QStringList& filter);

		QStringList countDepth(const Common::NODELIST& nodes,
						   int level);

		QStringList countEmpty(const Common::NODELIST& nodes,
						   int level, int action);

		QStringList countImages(const Common::NODELIST& nodes,
						    int level, int actions,
						    const QStringList& filter);

		QStringList countItems(const Common::NODELIST& nodes,
						   int level, int action, int col,
						   const QString& path);

		QStringList countTypes(const Common::NODELIST& nodes,
						   int level,
						   const QStringList& filter);

		QStringList performRemove(const Common::NODELIST& nodes,
							 int level, int action, int col,
							 const QString& path);

		QStringList performCopy(const Common::NODELIST& nodes,
						    int level, int action,
						    int col, int format,
						    const QString& path,
						    const QString& dest);

	protected:

		void start(void);
		void stop(void);

	public slots:

		void startProcessList(const QString& path,
						  const QString& logs,
						  const QVariantList& rules);

		void sendTerminateRequest(void);

	signals:

		void onJobChange(const QString&);

		void onJobStart(int, int);
		void onJobProgress(int);

		void onJobDone(const QStringList&);
		void onPartDone(const QStringList&);

};

#endif // THREADWORKER_HPP
