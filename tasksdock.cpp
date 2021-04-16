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

#include "tasksdock.hpp"
#include "ui_tasksdock.h"

TasksDock::TasksDock(QWidget *parent, bool loadlast)
	: AbstractDock("tasks", parent)
	, ui(new Ui::TasksDock)
{
	objectsFilter = tr("Tasks files (*.scvtasks);;All files (*.*)");
	objectFilter = tr("Task files (*.scvtask);;All files (*.*)");

	ui->setupUi(this);

	if (loadlast)
	{
		restoreFromSettings();
		refreshStatus();
	}

	QMenu* menu = new QMenu(tr("Select task"), this);
	QAction* separator = new QAction();

	actions = QList<QAction*>()
			<< new QAction(QIcon::fromTheme("edit-delete"), tr("Remove objects from file"), this)
			<< new QAction(QIcon::fromTheme("edit-copy"), tr("Copy objects from file"), this)
			<< separator
			<< new QAction(QIcon::fromTheme("document-new"), tr("Import object from file"), this)
			<< new QAction(QIcon::fromTheme("folder-new"), tr("Import list from file"), this);

	separator->setSeparator(true);
	menu->addActions(actions);

	ui->addButton->setMenu(menu);
	ui->scrollLayout->setAlignment(Qt::AlignTop);

	for (auto& a : actions) connect(a, &QAction::triggered, this, &TasksDock::addActionClicked);

	connect(ui->loadButton, &QToolButton::clicked,
		   this, &TasksDock::replaceDataDialog);

	connect(ui->saveButton, &QToolButton::clicked,
		   this, &TasksDock::saveDataDialog);

	connect(ui->runButton, &QToolButton::clicked,
		   this, &TasksDock::runButtonClicked);
}

TasksDock::~TasksDock(void)
{
	delete ui;
}

AbstractWidget* TasksDock::createWidget(const QString& name)
{
	if (name.isEmpty()) return nullptr;
	else if (name == "removejob") return new RemovejobWidget();
	else if (name == "copyjob") return new CopyjobWidget();
	else return nullptr;
}

AbstractWidget* TasksDock::createWidget(int id)
{
	switch (id)
	{
		case 0: return new RemovejobWidget();
		case 1: return new CopyjobWidget();
		case 3: return readWidget();
		case 4: appendDataDialog();
		default: return nullptr;
	}
}

bool TasksDock::appendWidget(WrapperWidget* widget)
{
	const bool ok = AbstractDock::appendWidget(widget);

	if (ok)
	{
		ui->scrollLayout->addWidget(widget);

		connect(widget, &WrapperWidget::onCheckChanged,
			   this, &TasksDock::refreshStatus);

		connect(widget, &WrapperWidget::onValidChanged,
			   this, &TasksDock::refreshStatus);

		refreshStatus();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Unable to add more tasks of this type"));

		widget->deleteLater();
	}

	return ok;
}

void TasksDock::widgetDeleted(void)
{
	AbstractDock::widgetDeleted(); refreshStatus();
}

void TasksDock::refreshStatus(void)
{
	const bool enableSave = !getValues(false, true).isEmpty();
	bool enableRun = !getValues(true, true).isEmpty();

	ui->runButton->setEnabled(enableRun);
	ui->saveButton->setEnabled(enableSave);
}

void TasksDock::addActionClicked(void)
{
	const int id = actions.indexOf(dynamic_cast<QAction*>(sender()));

	AbstractWidget* widget = createWidget(id);

	if (widget) appendWidget(wrappWidget(widget));
}

void TasksDock::runButtonClicked(void)
{
	const auto data = getValues(true, true);

	if (data.isEmpty()) return;
	else emit onTasksRequest(data);
}
