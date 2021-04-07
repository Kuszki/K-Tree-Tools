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

#include "emptyvalwidget.hpp"
#include "ui_emptyvalwidget.h"

EmptyvalWidget::EmptyvalWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("emptyval", parent, false)
	, ui(new Ui::EmptyvalWidget)
{
	ui->setupUi(this); setData(data);
}

EmptyvalWidget::~EmptyvalWidget(void)
{
	delete ui;
}

QVariantMap EmptyvalWidget::getData(void) const
{
	auto base = AbstractWidget::getData();

	base.insert(
	{
		{ "action", ui->actionCombo->currentIndex() },
		{ "objects", ui->objectCombo->currentIndex() },
		{ "level", ui->levelSpin->value() }
	});

	return base;
}

bool EmptyvalWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("action").toInt() >= 0 &&
			data.value("action").toInt() < ui->actionCombo->count() &&
			data.value("objects").toInt() >= 0 &&
			data.value("objects").toInt() < ui->objectCombo->count() &&
			data.value("level").toInt() >= ui->levelSpin->minimum() &&
			data.value("level").toInt() <= ui->levelSpin->maximum();
}

QString EmptyvalWidget::getDescriptionString(void) const
{
	const auto lvl = ui->levelSpin->value();

	return tr("%4 (level: %1, action: '%2', objects: '%3')")
			.arg(lvl == -1 ? tr("Any") : QString::number(lvl))
			.arg(ui->actionCombo->currentText())
			.arg(ui->objectCombo->currentText())
			.arg(getJobnameString());
}

QString EmptyvalWidget::getJobnameString(void) const
{
	return tr("Checking empty levels");
}

bool EmptyvalWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->actionCombo->setCurrentIndex(data.value("action").toInt());
	ui->objectCombo->setCurrentIndex(data.value("objects").toInt());
	ui->levelSpin->setValue(data.value("level").toInt());

	return AbstractWidget::setData(data);
}
