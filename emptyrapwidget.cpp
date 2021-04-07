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

#include "emptyrapwidget.hpp"
#include "ui_emptyrapwidget.h"

EmptyrapWidget::EmptyrapWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("emptyrap", parent, false)
	, ui(new Ui::EmptyrapWidget)
{
	ui->setupUi(this); setData(data);
}

EmptyrapWidget::~EmptyrapWidget(void)
{
	delete ui;
}

QVariantMap EmptyrapWidget::getData(void) const
{
	auto base = AbstractWidget::getData();

	base.insert(
	{
		{ "level", ui->levelSpin->value() },
		{ "action", ui->actionCombo->currentIndex() }
	});

	return base;
}

bool EmptyrapWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("action").toInt() >= 0 &&
			data.value("action").toInt() <= ui->actionCombo->count() &&
			data.value("level").toInt() >= ui->levelSpin->minimum() &&
			data.value("level").toInt() <= ui->levelSpin->maximum();
}

QString EmptyrapWidget::getDescriptionString(void) const
{
	const auto lvl = ui->levelSpin->value();

	return tr("%3 (level: %1, action: '%2')")
			.arg(lvl == -1 ? tr("Any") : QString::number(lvl))
			.arg(ui->actionCombo->currentText())
			.arg(getJobnameString());
}

QString EmptyrapWidget::getJobnameString(void) const
{
	return tr("Counting empty levels");
}

bool EmptyrapWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->levelSpin->setValue(data.value("level").toInt());
	ui->actionCombo->setCurrentIndex(data.value("action").toInt());

	return AbstractWidget::setData(data);
}
