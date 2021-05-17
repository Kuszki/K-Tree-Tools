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

#include "typesvalwidget.hpp"
#include "ui_typesvalwidget.h"

TypesvalWidget::TypesvalWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("typesval", parent, false)
	, ui(new Ui::TypesvalWidget)
{
	ui->setupUi(this); setData(data);

	filterStringChanged();

	connect(ui->extEdit, &QLineEdit::textChanged,
		   this, &TypesvalWidget::filterStringChanged);
}

TypesvalWidget::~TypesvalWidget(void)
{
	delete ui;
}

QVariantMap TypesvalWidget::getData(void) const
{
	auto base = AbstractWidget::getData();

	base.insert(
	{
		{ "action", ui->actionCombo->currentIndex() },
		{ "filter", getSelectedExtensions() },
		{ "level", ui->levelSpin->value() }
	});

	return base;
}

bool TypesvalWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("action").toInt() >= 0 &&
			data.value("action").toInt() < ui->actionCombo->count() &&
			data.value("level").toInt() >= ui->levelSpin->minimum() &&
			data.value("level").toInt() <= ui->levelSpin->maximum() &&
			!data.value("filter").toString().isEmpty();
}

QString TypesvalWidget::getDescriptionString(void) const
{
	const auto lvl = ui->levelSpin->value();

	return tr("%4 (level: %1, action: '%2', filter: '%3')")
			.arg(lvl == -1 ? tr("Any") : QString::number(lvl))
			.arg(ui->actionCombo->currentText())
			.arg(getSelectedExtensions().join(", "))
			.arg(getJobnameString());
}

QString TypesvalWidget::getJobnameString(void) const
{
	return tr("Ckecking file types");
}

QStringList TypesvalWidget::getSelectedExtensions(void) const
{
	auto text = ui->extEdit->text()
			  .replace(';', ',')
			  .replace(' ', ',');

	return text.split(',', Qt::SkipEmptyParts);
}

bool TypesvalWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->actionCombo->setCurrentIndex(data.value("action").toInt());
	ui->levelSpin->setValue(data.value("level").toInt());
	ui->extEdit->setText(data.value("filter").toStringList().join(", "));

	return AbstractWidget::setData(data);
}

void TypesvalWidget::filterStringChanged(void)
{
	const bool ok = !getSelectedExtensions().isEmpty();
	ui->extLabel->setStyleSheet(!ok ? wrongstyle : QString());

	emit onValidChanged(ok);
}
