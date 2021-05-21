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

#include "typesrapwidget.hpp"
#include "ui_typesrapwidget.h"

TypesrapWidget::TypesrapWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("typesrap", parent, false)
	, ui(new Ui::TypesrapWidget)
{
	ui->setupUi(this); setData(data);

	filterStringChanged();

	connect(ui->extEdit, &QLineEdit::textChanged,
		   this, &TypesrapWidget::filterStringChanged);
}

TypesrapWidget::~TypesrapWidget(void)
{
	delete ui;
}

QVariantMap TypesrapWidget::getData(void) const
{
	auto base = AbstractWidget::getData();

	base.insert(
	{
		{ "filter", getSelectedExtensions() },
		{ "level", ui->levelSpin->value() }
	});

	return base;
}

bool TypesrapWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("level").toInt() >= ui->levelSpin->minimum() &&
			data.value("level").toInt() <= ui->levelSpin->maximum() &&
			!data.value("filter").toString().isEmpty();
}

QString TypesrapWidget::getDescriptionString(void) const
{
	const auto lvl = ui->levelSpin->value();

	return tr("%3 (level: %1, filter: '%2')")
			.arg(lvl == -1 ? tr("Any", "level") : QString::number(lvl))
			.arg(getSelectedExtensions().join(", "))
			.arg(getJobnameString());
}

QString TypesrapWidget::getJobnameString(void) const
{
	return tr("Counting file types");
}

QStringList TypesrapWidget::getSelectedExtensions(void) const
{
	auto text = ui->extEdit->text()
			  .replace(';', ',')
			  .replace(' ', ',')
			  .split(',', Qt::SkipEmptyParts);

	text.removeDuplicates();

	return text;
}

bool TypesrapWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->levelSpin->setValue(data.value("level").toInt());
	ui->extEdit->setText(data.value("filter").toStringList().join(", "));

	return AbstractWidget::setData(data);
}

void TypesrapWidget::filterStringChanged(void)
{
	const bool ok = !getSelectedExtensions().isEmpty();
	ui->extLabel->setStyleSheet(!ok ? wrongstyle : QString());

	emit onValidChanged(ok);
}
