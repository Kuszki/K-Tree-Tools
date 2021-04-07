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

#include "wrapperwidget.hpp"
#include "ui_wrapperwidget.h"

WrapperWidget::WrapperWidget(AbstractWidget* widget, QWidget *parent, const QString& filter)
	: QWidget(parent)
	, ui(new Ui::WrapperWidget)
	, wrapped(widget)
	, fileFilter(filter)
{
	widget->setParent(this);

	ui->setupUi(this);

	ui->titleCheck->setText(widget->windowTitle());
	ui->titleCheck->setIcon(widget->windowIcon());
	ui->titleCheck->setChecked(widget->isActive());

	setWindowIcon(widget->windowIcon());
	setWindowIconText(widget->windowIconText());
	setWindowTitle(widget->windowTitle());

	ui->gridLayout->addWidget(widget,
						 ui->gridLayout->rowCount(),
						 0, 1,
						 ui->gridLayout->columnCount());

	connect(widget, &QWidget::destroyed,
		   this, &QWidget::deleteLater);

	connect(widget, &QWidget::windowIconChanged,
		   this, &QWidget::setWindowIcon);

	connect(widget, &QWidget::windowTitleChanged,
		   this, &QWidget::setWindowTitle);

	connect(widget, &QWidget::windowIconTextChanged,
		   this, &QWidget::setWindowIconText);

	connect(widget, &QWidget::windowIconChanged,
		   ui->titleCheck, &QCheckBox::setIcon);

	connect(this, &WrapperWidget::onCheckChanged,
		   widget, &AbstractWidget::setActive);

	connect(widget, &AbstractWidget::onActiveChanged,
		   this, &WrapperWidget::setChecked);

	connect(ui->deleteButton, &QToolButton::clicked,
		   this, &WrapperWidget::onUserDeleted);

	connect(ui->deleteButton, &QToolButton::clicked,
		   this, &WrapperWidget::deleteLater);

	connect(ui->saveButton, &QToolButton::clicked,
		   this, &WrapperWidget::saveButtonClicked);

	connect(ui->titleCheck, &QCheckBox::toggled,
		   this, &WrapperWidget::onCheckChanged);

	connect(ui->toggleButton, &QToolButton::toggled,
		   widget, &WrapperWidget::setVisible);
}

WrapperWidget::~WrapperWidget(void)
{
	delete ui;
}

const AbstractWidget* WrapperWidget::widget(void) const
{
	return wrapped;
}

AbstractWidget* WrapperWidget::widget(void)
{
	return wrapped;
}

bool WrapperWidget::isChecked(void) const
{
	return ui->titleCheck->isChecked();
}

bool WrapperWidget::isValid(void) const
{
	return wrapped->isValid();
}

QVariantMap WrapperWidget::getData(void) const
{
	return wrapped->getData();
}

void WrapperWidget::setChecked(bool checked)
{
	ui->titleCheck->setChecked(checked);
}

void WrapperWidget::saveButtonClicked(void)
{
	const auto path = QFileDialog::getSaveFileName(this,
					tr("Save object to file"),
					QString(), fileFilter);

	if (path.isEmpty()) return;

	if (!wrapped->saveData(path)) QMessageBox::critical(this,
		tr("Error"), tr("Unable to save file"));
}
