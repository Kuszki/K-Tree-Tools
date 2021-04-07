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

#include "treeviewdock.hpp"
#include "ui_treeviewdock.h"

TreeviewDock::TreeviewDock(QWidget *parent)
	: QDockWidget(parent)
	, ui(new Ui::TreeviewDock)
{
	ui->setupUi(this);

	model = new QFileSystemModel(this);
	model->setReadOnly(true);

	ui->treeView->model()->deleteLater();
	ui->treeView->setModel(model);

	QSettings Settings("K-OSP", "Walidator");

	Settings.beginGroup("Treeview");
	auto hs = Settings.value("header").toByteArray();
	Settings.endGroup();

	ui->treeView->header()->restoreState(hs);

	connect(ui->editButton, &QToolButton::toggled,
	[this] (bool en) -> void
	{
		model->setReadOnly(!en);
	});

	connect(ui->fopenButton, &QToolButton::clicked,
		   this, &TreeviewDock::fileButtonClicked);

	connect(ui->dopenButton, &QToolButton::clicked,
		   this, &TreeviewDock::dirButtonClicked);
}

TreeviewDock::~TreeviewDock(void)
{
	QSettings Settings("K-OSP", "Walidator");

	auto hs = ui->treeView->header()->saveState();

	Settings.beginGroup("Treeview");
	Settings.setValue("header", hs);
	Settings.endGroup();

	delete ui;
}

void TreeviewDock::selectRoot(const QString& path)
{
	model->setRootPath(path);
	auto i = model->index(path);
	ui->treeView->setRootIndex(i);
}

void TreeviewDock::fileButtonClicked(void)
{
	auto i = ui->treeView->selectionModel()->currentIndex();

	if (!i.isValid()) return;
	auto p = model->fileInfo(i);

	QDesktopServices::openUrl(QUrl::fromLocalFile(p.absoluteFilePath()));
}

void TreeviewDock::dirButtonClicked(void)
{
	auto i = ui->treeView->selectionModel()->currentIndex();

	if (!i.isValid()) return;
	auto p = model->fileInfo(i);

	QDesktopServices::openUrl(QUrl::fromLocalFile(p.absolutePath()));
}
