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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

#include "validatordock.hpp"
#include "treeviewdock.hpp"
#include "consoledock.hpp"
#include "raportdock.hpp"
#include "tasksdock.hpp"

#include "threadworker.hpp"

#include "aboutdialog.hpp"

#include "configwizard.hpp"

#include "common.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

		Q_OBJECT

	private:

		Ui::MainWindow *ui;

		QProgressBar* progress;
		QPushButton* terminator;

		ValidatorDock* validator;
		TreeviewDock* treeview;
		ConsoleDock* console;
		RaportDock* raport;
		TasksDock* tasks;

		QThread* wthread;
		ThreadWorker* worker;

		QString wname;
		QString root;
		QString logs;

	public:

		explicit MainWindow(QWidget *parent = nullptr);
		virtual ~MainWindow(void) override;

		void setRootPath(const QString& path);
		QString getRootPath(void) const;

		void setLogPath(const QString& path);
		QString getLogPath(void) const;

	private:

		bool monitIfWrongRoot(void);

	private slots:

		void aboutActionClicked(void);
		void saveActionClicked(void);
		void loadActionClicked(void);
		void clearActionClicked(void);
		void wizardActionClicked(void);

		void rootActionClicked(void);
		void logActionClicked(void);
		void runActionClicked(void);

		void dockOptionsChanged(void);

		void workerJobRequested(const QVariant& rule);
		void workerJobsRequested(const QVariantList& rules);
		void workerJobsDone(const QStringList& list);

		void wizardValuesAccepted(const QVariantMap& values);

		void lockWidgets(bool lock = true);

	signals:

		void onJobRequest(const QString&,
					   const QString&,
					   const QVariantList&);

		void onRootChanged(const QString&);
		void onLogsChanged(const QString&);

};

#endif // MAINWINDOW_HPP
