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

#include "deeprapwidget.hpp"
#include "ui_deeprapwidget.h"

DeeprapWidget::DeeprapWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("deeprap", parent, false)
	, ui(new Ui::DeeprapWidget)
{
	ui->setupUi(this); setData(data);
}

DeeprapWidget::~DeeprapWidget(void)
{
	delete ui;
}

QVariantMap DeeprapWidget::getData(void) const
{
	auto base = AbstractWidget::getData();

	base.insert(
	{
		{ "level", ui->levelSpin->value() }
	});

	return base;
}

bool DeeprapWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("level").toInt() >= ui->levelSpin->minimum() &&
			data.value("level").toInt() <= ui->levelSpin->maximum();
}

QString DeeprapWidget::getDescriptionString(void) const
{
	const auto lvl = ui->levelSpin->value();

	return tr("%2 (level: %1)")
			.arg(lvl == -1 ? tr("Root") : QString::number(lvl))
			.arg(getJobnameString());
}

QString DeeprapWidget::getJobnameString(void) const
{
	return tr("Counting tree depth");
}

bool DeeprapWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->levelSpin->setValue(data.value("level").toInt());

	return AbstractWidget::setData(data);
}
