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

#include "validatordock.hpp"
#include "ui_validatordock.h"

ValidatorDock::ValidatorDock(QWidget *parent, bool loadlast)
	: AbstractDock("validators", parent)
	, ui(new Ui::ValidatorDock)
{
	objectsFilter = tr("Rules files (*.scvrules);;All files (*.*)");
	objectFilter = tr("Rule files (*.scvrule);;All files (*.*)");

	ui->setupUi(this);

	if (loadlast)
	{
		restoreFromSettings();
		refreshStatus();
	}

	QMenu* menu = new QMenu(tr("Select validator"), this);
	QAction* separator = new QAction();

	actions = QList<QAction*>()
			<< new QAction(QIcon::fromTheme("view-sort-ascending"), tr("Check tree depth"), this)
			<< new QAction(QIcon::fromTheme("folder-new"), tr("Ckeck empty directories"), this)
			<< new QAction(QIcon::fromTheme("document-properties"), tr("Ckeck image parameters"), this)
			<< new QAction(QIcon::fromTheme("edit-select-all"), tr("Ckeck objects from file"), this)
			<< new QAction(QIcon::fromTheme("format-indent-more"), tr("Ckeck tree from file"), this)
			<< new QAction(QIcon::fromTheme("format-text-italic"), tr("Check file types"), this)
			<< separator
			<< new QAction(QIcon::fromTheme("document-new"), tr("Import object from file"), this)
			<< new QAction(QIcon::fromTheme("folder-new"), tr("Import list from file"), this);

	separator->setSeparator(true);
	menu->addActions(actions);

	ui->addButton->setMenu(menu);
	ui->scrollLayout->setAlignment(Qt::AlignTop);

	for (auto& a : actions) connect(a, &QAction::triggered, this, &ValidatorDock::addActionClicked);

	connect(ui->loadButton, &QToolButton::clicked,
		   this, &ValidatorDock::replaceDataDialog);

	connect(ui->saveButton, &QToolButton::clicked,
		   this, &ValidatorDock::saveDataDialog);

	connect(ui->runButton, &QToolButton::clicked,
		   this, &ValidatorDock::runButtonClicked);
}

ValidatorDock::~ValidatorDock(void)
{
	delete ui;
}

AbstractWidget* ValidatorDock::createWidget(const QString& name)
{
	if (name.isEmpty()) return nullptr;
	else if (name == "deepval") return new DeepvalWidget();
	else if (name == "emptyval") return new EmptyvalWidget();
	else if (name == "imageval") return new ImagevalWidget();
	else if (name == "itemsval") return new ItemsvalWidget();
	else if (name == "treeval") return new TreevalWidget();
	else if (name == "typesval") return new TypesvalWidget();
	else return nullptr;
}

AbstractWidget* ValidatorDock::createWidget(int id)
{
	switch (id)
	{
		case 0: return new DeepvalWidget();
		case 1: return new EmptyvalWidget();
		case 2: return new ImagevalWidget();
		case 3: return new ItemsvalWidget();
		case 4: return new TreevalWidget();
		case 5: return new TypesvalWidget();
		case 7: return readWidget();
		case 8: appendDataDialog();
		default: return nullptr;
	}
}

bool ValidatorDock::appendWidget(WrapperWidget* widget)
{
	const bool ok = AbstractDock::appendWidget(widget);

	if (ok)
	{
		ui->scrollLayout->addWidget(widget);

		connect(widget, &WrapperWidget::onCheckChanged,
			   this, &ValidatorDock::refreshStatus);

		refreshStatus();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Unable to add more rules of this type"));

		widget->deleteLater();
	}

	return ok;
}

void ValidatorDock::widgetDeleted(void)
{
	AbstractDock::widgetDeleted(); refreshStatus();
}

void ValidatorDock::refreshStatus(void)
{
	const bool enableSave = !getValues(false, true).isEmpty();
	bool enableRun = !getValues(true, true).isEmpty();

	ui->runButton->setEnabled(enableRun);
	ui->saveButton->setEnabled(enableSave);
}

void ValidatorDock::addActionClicked(void)
{
	const int id = actions.indexOf(dynamic_cast<QAction*>(sender()));

	AbstractWidget* widget = createWidget(id);

	if (widget) appendWidget(wrappWidget(widget));
}

void ValidatorDock::runButtonClicked(void)
{
	const auto data = getValues(true, true);

	if (data.isEmpty()) return;
	else emit onValidationRequest(data);
}
