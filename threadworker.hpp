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

#include <Magick++.h>

#include "common.hpp"

class ThreadWorker : public QObject
{

		Q_OBJECT

	private:

		const QChar fsep = ',';

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
							 Common::NODEPTR parent = nullptr) const;

		QVariantList getTable(const QString& path, int col = -1) const;

		QStringList validateDepth(const Common::NODELIST& nodes,
							 int min, int max, int files,
							 const QString& objDesc = QString());

		QStringList validateEmpty(const Common::NODELIST& nodes,
							 int level, int action, int obj,
							 const QString& objDesc = QString());

		QStringList validateImages(const Common::NODELIST& nodes,
							  int level, int dpi,
							  int minqual, int maxqual,
							  const QStringList& filter,
							  const QString& objDesc = QString());

		QStringList validateItems(const Common::NODELIST& nodes,
							 int level, int action, int col,
							 const QString& path,
							 const QString& objDesc = QString());

		QStringList validateTree(const Common::NODELIST& nodes,
							int format, int action,
							const QString& path,
							const QString& objDesc = QString());

		QStringList validateTypes(const Common::NODELIST& nodes,
							 int level, int action,
							 const QStringList& filter,
							 const QString& objDesc = QString());

		QStringList countDepth(const Common::NODELIST& nodes,
						   int level,
						   const QString& objDesc = QString());

		QStringList countEmpty(const Common::NODELIST& nodes,
						   int level, int action,
						   const QString& objDesc = QString());

		QStringList countImages(const Common::NODELIST& nodes,
						    int level, int actions,
						    const QStringList& filter,
						    const QString& objDesc = QString());

		QStringList countItems(const Common::NODELIST& nodes,
						   int level, int action, int col,
						   const QString& path,
						   const QString& objDesc = QString());

		QStringList countTypes(const Common::NODELIST& nodes,
						   int level,
						   const QStringList& filter,
						   const QString& objDesc = QString());

		QStringList performRemove(const Common::NODELIST& nodes,
							 int level, int action, int col,
							 const QString& path,
							 const QString& objDesc = QString());

		QStringList performCopy(const Common::NODELIST& nodes,
						    int level, int action,
						    int col, int format,
						    const QString& path,
						    const QString& dest,
						    const QString& objDesc = QString());

		static QString getTimestamp(void);

	protected:

		bool saveLogs(const QString& name,
				    const QStringList& log);

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
