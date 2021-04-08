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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	QSettings Settings("K-OSP", "Walidator");

	Settings.beginGroup("Settings");
	root = Settings.value("root").toString();
	bool al = Settings.value("autoload").toBool();
	bool as = Settings.value("autosave").toBool();
	Settings.endGroup();

	ui->setupUi(this);

	wname = windowTitle();

	ui->centralwidget->deleteLater();
	ui->actionAutorestore->setChecked(al);
	ui->actionAutosave->setChecked(as);

	wthread = new QThread(this);
	worker = new ThreadWorker();
	worker->moveToThread(wthread);
	wthread->start();

	terminator = new QPushButton(QIcon::fromTheme("process-stop"), tr("Stop"), this);
	terminator->hide();

	progress = new QProgressBar(this);
	progress->hide();

	ui->statusbar->addPermanentWidget(progress);
	ui->statusbar->addPermanentWidget(terminator);

	validator = new ValidatorDock(this, al);
	raport = new RaportDock(this, al);
	tasks = new TasksDock(this, al);

	treeview = new TreeviewDock(this);
	console = new ConsoleDock(this);

	validator->setAutosave(as);
	raport->setAutosave(as);
	tasks->setAutosave(as);

	addDockWidget(Qt::LeftDockWidgetArea, validator);
	addDockWidget(Qt::LeftDockWidgetArea, raport);
	addDockWidget(Qt::LeftDockWidgetArea, tasks);

	addDockWidget(Qt::RightDockWidgetArea, treeview);
	addDockWidget(Qt::RightDockWidgetArea, console);

	Settings.beginGroup("Window");
	setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::TabPosition::North);
	restoreGeometry(Settings.value("geometry").toByteArray());
	restoreState(Settings.value("state").toByteArray());
	Settings.endGroup();

	if (isMaximized()) setGeometry(QApplication::desktop()->availableGeometry(this));

	if (!root.isEmpty() && QDir(root).exists())
	{
		setWindowTitle(QString("%1 (%2)")
					.arg(wname).arg(root));

		treeview->selectRoot(root);
	}

	lockWidgets(false);

	connect(terminator, &QPushButton::clicked,
		   worker, &ThreadWorker::sendTerminateRequest);

	connect(terminator, &QPushButton::clicked,
		   terminator, &QPushButton::hide);

	connect(validator, &ValidatorDock::onValidationRequest,
		   this, &MainWindow::workerJobRequested);

	connect(tasks, &TasksDock::onTasksRequest,
		   this, &MainWindow::workerJobRequested);

	connect(raport, &RaportDock::onRaportRequest,
		   this, &MainWindow::workerJobRequested);

	connect(worker, &ThreadWorker::onJobStart,
		   progress, &QProgressBar::setRange);

	connect(worker, &ThreadWorker::onJobProgress,
		   progress, &QProgressBar::setValue);

	connect(worker, &ThreadWorker::onPartDone,
		   console, &ConsoleDock::appendMessages);

	connect(this, &MainWindow::onRootChanged,
		   treeview, &TreeviewDock::selectRoot);

	connect(worker, &ThreadWorker::onJobDone,
		   this, &MainWindow::workerJobDone);

	connect(this, &MainWindow::onJobRequest,
		   worker, &ThreadWorker::startProcessList);

	connect(ui->actionRoot, &QAction::triggered,
		   this, &MainWindow::rootActionClicked);

	connect(ui->actionRun, &QAction::triggered,
		   this, &MainWindow::runActionClicked);

	connect(ui->actionAutosave, &QAction::toggled,
		   validator, &ValidatorDock::setAutosave);

	connect(ui->actionAutosave, &QAction::toggled,
		   tasks, &TasksDock::setAutosave);

	connect(ui->actionAutosave, &QAction::toggled,
		   raport, &RaportDock::setAutosave);

	connect(ui->actionLoadall, &QAction::triggered,
		   this, &MainWindow::loadActionClicked);

	connect(ui->actionSaveall, &QAction::triggered,
		   this, &MainWindow::saveActionClicked);

	connect(ui->actionClearall, &QAction::triggered,
		   this, &MainWindow::clearActionClicked);

	connect(worker, &ThreadWorker::onJobChange,
	[this] (const QString& msg) -> void
	{
		ui->statusbar->showMessage(msg);
	});
}

MainWindow::~MainWindow(void)
{
	QSettings Settings("K-OSP", "Walidator");

	Settings.beginGroup("Window");
	Settings.setValue("state", saveState());
	Settings.setValue("geometry", saveGeometry());
	Settings.endGroup();

	Settings.beginGroup("Settings");
	Settings.setValue("root", root);
	Settings.setValue("autoload", ui->actionAutorestore->isChecked());
	Settings.setValue("autosave", ui->actionAutosave->isChecked());
	Settings.endGroup();

	worker->sendTerminateRequest();

	wthread->exit();
	wthread->wait();

	delete worker;
	delete ui;
}

bool MainWindow::monitIfWrongRoot(void)
{
	if (root.isEmpty() || !QDir(root).exists())
	{
		rootActionClicked();
	}

	if (root.isEmpty() || !QDir(root).exists())
	{
		return false;
	}

	return true;
}

void MainWindow::saveActionClicked(void)
{
	const auto vals = validator->getValues();
	const auto tass = tasks->getValues();
	const auto raps = raport->getValues();

	if (!(vals.size() + tass.size() + raps.size()))
	{
		QMessageBox::warning(this, tr("Error"),
			tr("There is no tasks to save"));

		return;
	}

	const QString path = QFileDialog::getSaveFileName(this,
				tr("Save tasks to file"), QString(),
				tr("Tasks lists (*.scvlist);;All files (*.*)"));

	if (path.isEmpty()) return;

	QVariantMap map;
	map.insert("vals", vals);
	map.insert("tass", tass);
	map.insert("raps", raps);

	QFile file(path);

	if (!file.open(QFile::WriteOnly))
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Unable to create selected file"));

		return;
	}

	QDataStream stream(&file);
	stream << map;
}

void MainWindow::loadActionClicked(void)
{
	const QString path = QFileDialog::getOpenFileName(this,
				tr("Save tasks to file"), QString(),
				tr("Tasks lists (*.scvlist);;All files (*.*)"));

	if (path.isEmpty()) return;

	QFile file(path);

	if (!file.open(QFile::ReadOnly))
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Unable to open selected file"));

		return;
	}

	QDataStream stream(&file);
	QVariantMap map;

	stream >> map;

	validator->setValues(map.value("vals").toList(), true);
	tasks->setValues(map.value("tass").toList(), true);
	raport->setValues(map.value("raps").toList(), true);
}

void MainWindow::clearActionClicked(void)
{
	validator->clearWidgets();
	tasks->clearWidgets();
	raport->clearWidgets();
}

void MainWindow::setRootPath(const QString& path)
{
	setWindowTitle(QString("%1 (%2)").arg(wname).arg(path));
	emit onRootChanged(root = path);
}

QString MainWindow::getRootPath(void) const
{
	return root;
}

void MainWindow::rootActionClicked(void)
{
	const QString path = QFileDialog::getExistingDirectory(this,
		tr("Select root directory"), root);

	if (!path.isEmpty()) setRootPath(path);
}

void MainWindow::runActionClicked(void)
{
	QVariantList rules;

	rules.append(tasks->getValues(true));
	rules.append(validator->getValues(true));
	rules.append(raport->getValues(true));

	workerJobRequested(rules);
}

void MainWindow::workerJobRequested(const QVariantList& rules)
{
	if (!worker->isStartable()) return;

	if (rules.isEmpty()) QMessageBox::warning(this, tr("Error"),
			tr("No active rules or tasks present"));
	else if (monitIfWrongRoot())
	{
		lockWidgets(true); emit onJobRequest(root, rules);
	}
	else QMessageBox::warning(this, tr("Error"),
			tr("No existing root directory selected"));
}

void MainWindow::workerJobDone(const QStringList&)
{
	lockWidgets(false);
}

void MainWindow::lockWidgets(bool lock)
{
	const bool dirOK = !root.isEmpty() && QDir(root).exists();

	progress->setVisible(lock);
	terminator->setVisible(lock);

	validator->setEnabled(!lock);
	treeview->setEnabled(!lock);
	console->setEnabled(!lock);
	raport->setEnabled(!lock);
	tasks->setEnabled(!lock);

	ui->actionRun->setEnabled(!lock && dirOK);
}

