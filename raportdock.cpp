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

#include "raportdock.hpp"
#include "ui_raportdock.h"

RaportDock::RaportDock(QWidget *parent, bool loadlast)
	: AbstractDock("raports", parent)
	, ui(new Ui::RaportDock)
{
	objectsFilter = tr("Raports files (*.scvraps);;All files (*.*)");
	objectFilter = tr("Raport files (*.scvrap);;All files (*.*)");

	ui->setupUi(this);

	if (loadlast)
	{
		restoreFromSettings();
		refreshStatus();
	}

	QMenu* menu = new QMenu(tr("Select raport"), this);
	QAction* separator = new QAction();

	actions = QList<QAction*>()
			<< new QAction(QIcon::fromTheme("view-sort-ascending"), tr("Count tree depth"), this)
			<< new QAction(QIcon::fromTheme("folder-new"), tr("Count empty directories"), this)
			<< new QAction(QIcon::fromTheme("document-properties"), tr("Count image parameters"), this)
			<< new QAction(QIcon::fromTheme("edit-select-all"), tr("Count objects in tree"), this)
			<< new QAction(QIcon::fromTheme("format-text-italic"), tr("Count file types"), this)
			<< separator
			<< new QAction(QIcon::fromTheme("document-new"), tr("Import object from file"), this)
			<< new QAction(QIcon::fromTheme("folder-new"), tr("Import list from file"), this);

	separator->setSeparator(true);
	menu->addActions(actions);

	ui->addButton->setMenu(menu);
	ui->scrollLayout->setAlignment(Qt::AlignTop);

	for (auto& a : actions) connect(a, &QAction::triggered, this, &RaportDock::addActionClicked);

	connect(ui->loadButton, &QToolButton::clicked,
		   this, &RaportDock::replaceDataDialog);

	connect(ui->saveButton, &QToolButton::clicked,
		   this, &RaportDock::saveDataDialog);

	connect(ui->runButton, &QToolButton::clicked,
		   this, &RaportDock::runButtonClicked);
}

RaportDock::~RaportDock(void)
{
	delete ui;
}

AbstractWidget* RaportDock::createWidget(const QString& name)
{
	if (name.isEmpty()) return nullptr;
	else if (name == "deeprap") return new DeeprapWidget();
	else if (name == "emptyrap") return new EmptyrapWidget();
	else if (name == "imagerap") return new ImagerapWidget();
	else if (name == "itemsrap") return new ItemsrapWidget();
	else if (name == "typesrap") return new TypesrapWidget();
	else return nullptr;
}

AbstractWidget* RaportDock::createWidget(int id)
{
	switch (id)
	{
		case 0: return new DeeprapWidget();
		case 1: return new EmptyrapWidget();
		case 2: return new ImagerapWidget();
		case 3: return new ItemsrapWidget();
		case 4: return new TypesrapWidget();
		case 6: return readWidget();
		case 7: appendDataDialog();
		default: return nullptr;
	}
}

bool RaportDock::appendWidget(WrapperWidget* widget)
{
	const bool ok = AbstractDock::appendWidget(widget);

	if (ok)
	{
		ui->scrollLayout->addWidget(widget);

		connect(widget, &WrapperWidget::onCheckChanged,
			   this, &RaportDock::refreshStatus);

		refreshStatus();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Unable to add more raports of this type"));

		widget->deleteLater();
	}

	return ok;
}

void RaportDock::widgetDeleted(void)
{
	AbstractDock::widgetDeleted(); refreshStatus();
}

void RaportDock::refreshStatus(void)
{
	const bool enableSave = !getValues(false, true).isEmpty();
	bool enableRun = !getValues(true, true).isEmpty();

	ui->runButton->setEnabled(enableRun);
	ui->saveButton->setEnabled(enableSave);
}

void RaportDock::addActionClicked(void)
{
	const int id = actions.indexOf(dynamic_cast<QAction*>(sender()));

	AbstractWidget* widget = createWidget(id);

	if (widget) appendWidget(wrappWidget(widget));
}

void RaportDock::runButtonClicked(void)
{
	const auto data = getValues(true, true);

	if (data.isEmpty()) return;
	else emit onRaportRequest(data);
}
