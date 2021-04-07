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

#include "deepvalwidget.hpp"
#include "ui_deepvalwidget.h"

DeepvalWidget::DeepvalWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("deepval", parent, true)
	, ui(new Ui::DeepvalWidget)
{
	ui->setupUi(this); setData(data);

	ui->minSpin->setSuffix(tr(" level(s)", nullptr, ui->minSpin->value()));
	ui->maxSpin->setSuffix(tr(" level(s)", nullptr, ui->maxSpin->value()));

	ui->maxSpin->setMinimum(ui->minSpin->value());
	ui->minSpin->setMaximum(ui->maxSpin->value());
	ui->allowSpin->setMaximum(ui->maxSpin->value());

	connect(ui->minSpin, qOverload<int>(&QSpinBox::valueChanged),
		   this, &DeepvalWidget::spinValueChanged);

	connect(ui->maxSpin, qOverload<int>(&QSpinBox::valueChanged),
		   this, &DeepvalWidget::spinValueChanged);
}

DeepvalWidget::~DeepvalWidget(void)
{
	delete ui;
}

QVariantMap DeepvalWidget::getData(void) const
{
	auto base = AbstractWidget::getData();

	base.insert(
	{
		{ "min", ui->minSpin->value() },
		{ "max", ui->maxSpin->value() },
		{ "files", ui->allowSpin->value() }
	});

	return base;
}

bool DeepvalWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("min").toInt() >= ui->minSpin->minimum() &&
			data.value("max").toInt() <= ui->maxSpin->maximum() &&
			data.value("files").toInt() >= ui->allowSpin->minimum() &&
			data.value("files").toInt() <= ui->allowSpin->maximum() &&
			data.value("min").toInt() <= data.value("max").toInt() &&
			data.value("files").toInt() <= data.value("max").toInt();
}

QString DeepvalWidget::getDescriptionString(void) const
{
	return tr("%3 (minimum: %1, maximum: %2)")
			.arg(ui->minSpin->value())
			.arg(ui->maxSpin->value())
			.arg(getJobnameString());
}

QString DeepvalWidget::getJobnameString(void) const
{
	return tr("Checking tree depth");
}

bool DeepvalWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->minSpin->setValue(data.value("min").toInt());
	ui->maxSpin->setValue(data.value("max").toInt());
	ui->allowSpin->setValue(data.value("files").toInt());

	return AbstractWidget::setData(data);
}

void DeepvalWidget::spinValueChanged(int value)
{
	auto spin = dynamic_cast<QSpinBox*>(sender());
	spin->setSuffix(tr(" level(s)", nullptr, value));

	ui->maxSpin->setMinimum(ui->minSpin->value());
	ui->minSpin->setMaximum(ui->maxSpin->value());
	ui->allowSpin->setMaximum(ui->maxSpin->value());
}
