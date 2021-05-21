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

#include "removejobwidget.hpp"
#include "ui_removejobwidget.h"

RemovejobWidget::RemovejobWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("removejob", parent, false)
	, ui(new Ui::RemovejobWidget)
{
	ui->setupUi(this); setData(data);

	pathStringChanged(ui->pathEdit->text());

	connect(ui->pathEdit, &QLineEdit::textChanged,
		   this, &RemovejobWidget::pathStringChanged);

	connect(ui->pathButton, &QToolButton::clicked,
		   this, &RemovejobWidget::openButtonClicked);
}

RemovejobWidget::~RemovejobWidget(void)
{
	delete ui;
}

QVariantMap RemovejobWidget::getData(void) const
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

bool RemovejobWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("action").toInt() >= 0 &&
			data.value("action").toInt() < ui->actionCombo->count() &&
			data.value("level").toInt() >= ui->levelSpin->minimum() &&
			data.value("level").toInt() <= ui->levelSpin->maximum() &&
			data.value("column").toInt() >= ui->columnSpin->minimum() &&
			data.value("column").toInt() <= ui->columnSpin->maximum() &&
			!data.value("path").toString().isEmpty();
}

QString RemovejobWidget::getDescriptionString(void) const
{
	const auto lvl = ui->levelSpin->value();

	return tr("%5 (level: %1, action: '%2', file: '%3', column: %4)")
			.arg(lvl == -1 ? tr("Any", "level") : QString::number(lvl))
			.arg(ui->actionCombo->currentText())
			.arg(ui->pathEdit->text())
			.arg(ui->columnSpin->value())
			.arg(getJobnameString());
}

QString RemovejobWidget::getJobnameString(void) const
{
	return tr("Removing objects from file");
}

bool RemovejobWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->actionCombo->setCurrentIndex(data.value("action").toInt());
	ui->levelSpin->setValue(data.value("level").toInt());
	ui->columnSpin->setValue(data.value("column").toInt());
	ui->pathEdit->setText(data.value("path").toString());

	return AbstractWidget::setData(data);
}

void RemovejobWidget::pathStringChanged(const QString& path)
{
	const bool ok = !path.isEmpty();
	ui->pathLabel->setStyleSheet(!ok ? wrongstyle : QString());

	emit onValidChanged(ok);
}

void RemovejobWidget::openButtonClicked(void)
{
	const QString path = QFileDialog::getOpenFileName(this,
			tr("Select source file"), QString(),
			tr("Text files (*.txt);;CSV files (*.csv);;All files (*.*)"));

	if (!path.isEmpty()) ui->pathEdit->setText(path);
}
