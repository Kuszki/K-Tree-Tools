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

#include "copyjobwidget.hpp"
#include "ui_copyjobwidget.h"

CopyjobWidget::CopyjobWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("copyjob", parent, false)
	, ui(new Ui::CopyjobWidget)
{
	ui->setupUi(this); setData(data);

	pathStringChanged();

	connect(ui->srcpathEdit, &QLineEdit::textChanged,
		   this, &CopyjobWidget::pathStringChanged);

	connect(ui->dstpathEdit, &QLineEdit::textChanged,
		   this, &CopyjobWidget::pathStringChanged);

	connect(ui->srcpathButton, &QToolButton::clicked,
		   this, &CopyjobWidget::srcopenButtonClicked);

	connect(ui->dstpathButton, &QToolButton::clicked,
		   this, &CopyjobWidget::dstopenButtonClicked);
}

CopyjobWidget::~CopyjobWidget(void)
{
	delete ui;
}

QVariantMap CopyjobWidget::getData(void) const
{
	auto base = AbstractWidget::getData();

	base.insert(
	{
		{ "action", ui->actionCombo->currentIndex() },
		{ "format", ui->formatCombo->currentIndex() },
		{ "level", ui->levelSpin->value() },
		{ "path", ui->srcpathEdit->text() },
		{ "column", ui->columnSpin->value() },
		{ "dest", ui->dstpathEdit->text() },
	});

	return base;
}

bool CopyjobWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("action").toInt() >= 0 &&
			data.value("action").toInt() < ui->actionCombo->count() &&
			data.value("format").toInt() >= 0 &&
			data.value("format").toInt() < ui->formatCombo->count() &&
			data.value("level").toInt() >= ui->levelSpin->minimum() &&
			data.value("level").toInt() <= ui->levelSpin->maximum() &&
			data.value("column").toInt() >= ui->columnSpin->minimum() &&
			data.value("column").toInt() <= ui->columnSpin->maximum() &&
			!data.value("path").toString().isEmpty() &&
			!data.value("dest").toString().isEmpty();
}

QString CopyjobWidget::getDescriptionString(void) const
{
	const auto lvl = ui->levelSpin->value();

	return tr("%7 (level: %1, action: '%2', format: '%3' file: '%4', column: %5, destination: '%6')")
			.arg(lvl == -1 ? tr("Any") : QString::number(lvl))
			.arg(ui->actionCombo->currentText())
			.arg(ui->formatCombo->currentText())
			.arg(ui->srcpathEdit->text())
			.arg(ui->columnSpin->value())
			.arg(ui->dstpathEdit->text())
			.arg(getJobnameString());
}

QString CopyjobWidget::getJobnameString(void) const
{
	return tr("Copying objects from file");
}

bool CopyjobWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->actionCombo->setCurrentIndex(data.value("action").toInt());
	ui->formatCombo->setCurrentIndex(data.value("format").toInt());
	ui->levelSpin->setValue(data.value("level").toInt());
	ui->columnSpin->setValue(data.value("column").toInt());
	ui->srcpathEdit->setText(data.value("path").toString());
	ui->dstpathEdit->setText(data.value("dest").toString());

	return AbstractWidget::setData(data);
}

void CopyjobWidget::pathStringChanged(void)
{
	const bool sok = !ui->srcpathEdit->text().isEmpty();
	ui->srcpathLabel->setStyleSheet(!sok ? wrongstyle : QString());

	const bool dok = !ui->dstpathEdit->text().isEmpty();
	ui->dstpathLabel->setStyleSheet(!dok ? wrongstyle : QString());

	emit onValidChanged(sok && dok);
}

void CopyjobWidget::srcopenButtonClicked(void)
{
	const QString path = QFileDialog::getOpenFileName(this,
			tr("Select source file"), QString(),
			tr("Text files (*.txt);;CSV files (*.csv);;All files (*.*)"));

	if (!path.isEmpty()) ui->srcpathEdit->setText(path);
}

void CopyjobWidget::dstopenButtonClicked(void)
{
	const QString path = QFileDialog::getExistingDirectory(this,
			tr("Select destination directory"),
			ui->dstpathEdit->text());

	if (!path.isEmpty()) ui->dstpathEdit->setText(path);
}
