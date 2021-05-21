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

#include "itemsrapwidget.hpp"
#include "ui_itemsrapwidget.h"

ItemsrapWidget::ItemsrapWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("itemsrap", parent)
	, ui(new Ui::ItemsrapWidget)
{
	ui->setupUi(this); setData(data);

	actionIndexChanged(ui->actionCombo->currentIndex());
	pathStringChanged(ui->pathEdit->text());

	connect(ui->pathButton, &QToolButton::clicked,
		   this, &ItemsrapWidget::openButtonClicked);

	connect(ui->pathEdit, &QLineEdit::textChanged,
		   this, &ItemsrapWidget::pathStringChanged);

	connect(ui->actionCombo, qOverload<int>(&QComboBox::currentIndexChanged),
		   this, &ItemsrapWidget::actionIndexChanged);
}

ItemsrapWidget::~ItemsrapWidget(void)
{
	delete ui;
}

QVariantMap ItemsrapWidget::getData(void) const
{
	auto base = AbstractWidget::getData();

	base.insert(
	{
		{ "action", ui->actionCombo->currentIndex() },
		{ "level", ui->levelSpin->value() },
		{ "path", ui->pathEdit->text() },
		{ "column", ui->columnSpin->value() }
	});

	return base;
}

bool ItemsrapWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("column").toInt() >= ui->columnSpin->minimum() &&
			data.value("column").toInt() <= ui->columnSpin->maximum() &&
			data.value("level").toInt() >= ui->levelSpin->minimum() &&
			data.value("level").toInt() <= ui->levelSpin->maximum() &&
			(data.value("action").toInt() == 0 ||
			 !data.value("path").toString().isEmpty());
}

QString ItemsrapWidget::getDescriptionString(void) const
{
	const auto lvl = ui->levelSpin->value();

	if (ui->actionCombo->currentIndex() != 0)
		return tr("%5 (level: %1, action: '%2', file: '%3', column: %4)")
				.arg(lvl == -1 ? tr("Any", "level") : QString::number(lvl))
				.arg(ui->actionCombo->currentText())
				.arg(ui->pathEdit->text())
				.arg(ui->columnSpin->value())
				.arg(getJobnameString());
	else
		return tr("%3 (level: %1, action: '%2')")
			.arg(lvl == -1 ? tr("Any") : QString::number(lvl))
			.arg(ui->actionCombo->currentText())
			.arg(getJobnameString());
}

QString ItemsrapWidget::getJobnameString(void) const
{
	return tr("Counting objects in tree");
}

bool ItemsrapWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->actionCombo->setCurrentIndex(data.value("action").toInt());
	ui->levelSpin->setValue(data.value("level").toInt());
	ui->columnSpin->setValue(data.value("column").toInt());
	ui->pathEdit->setText(data.value("path").toString());

	return AbstractWidget::setData(data);
}

void ItemsrapWidget::pathStringChanged(const QString& path)
{
	const bool ok = !path.isEmpty() || !ui->actionCombo->currentIndex();
	ui->pathLabel->setStyleSheet(!ok ? wrongstyle : QString());

	emit onValidChanged(ok);
}

void ItemsrapWidget::actionIndexChanged(int index)
{
	pathStringChanged(ui->pathEdit->text());

	ui->columnLabel->setEnabled(index);
	ui->columnSpin->setEnabled(index);

	ui->pathLabel->setEnabled(index);
	ui->pathButton->setEnabled(index);
	ui->pathEdit->setEnabled(index);
}

void ItemsrapWidget::openButtonClicked(void)
{
	const QString path = QFileDialog::getOpenFileName(this,
			tr("Select source file"), QString(),
			tr("Text files (*.txt);;CSV files (*.csv);;All files (*.*)"));

	if (!path.isEmpty()) ui->pathEdit->setText(path);
}
