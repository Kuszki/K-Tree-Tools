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

#include "treevalwidget.hpp"
#include "ui_treevalwidget.h"

TreevalWidget::TreevalWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("treeval", parent, true)
	, ui(new Ui::TreevalWidget)
{
	ui->setupUi(this); setData(data);

	connect(ui->pathButton, &QToolButton::clicked,
		   this, &TreevalWidget::openButtonClicked);

	connect(ui->pathEdit, &QLineEdit::textChanged,
		   this, &TreevalWidget::pathStringChanged);
}

TreevalWidget::~TreevalWidget(void)
{
	delete ui;
}

QVariantMap TreevalWidget::getData(void) const
{
	auto base = AbstractWidget::getData();

	base.insert(
	{
		{ "path", ui->pathEdit->text() },
		{ "format", ui->formatCombo->currentIndex() },
		{ "action", ui->actionCombo->currentIndex() }
	});

	return base;
}

bool TreevalWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("format").toInt() >= 0 &&
			data.value("format").toInt() < ui->formatCombo->count() &&
			data.value("action").toInt() >= 0 &&
			data.value("action").toInt() < ui->actionCombo->count() &&
			!data.value("path").toString().isEmpty();
}

QString TreevalWidget::getDescriptionString(void) const
{
	return tr("%3 (action: '%1', file: '%2')")
			.arg(ui->actionCombo->currentText())
			.arg(ui->pathEdit->text())
			.arg(getJobnameString());
}

QString TreevalWidget::getJobnameString(void) const
{
	return tr("Ckecking tree from file");
}

bool TreevalWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->pathEdit->setText(data.value("path").toString());
	ui->formatCombo->setCurrentIndex(data.value("format").toInt());
	ui->actionCombo->setCurrentIndex(data.value("action").toInt());

	return AbstractWidget::setData(data);
}

void TreevalWidget::pathStringChanged(const QString& path)
{
	const bool ok = !path.isEmpty();
	ui->pathLabel->setStyleSheet(!ok ? wrongstyle : QString());
}

void TreevalWidget::openButtonClicked(void)
{
	const QString path = QFileDialog::getOpenFileName(this,
			tr("Select source file"), QString(),
			tr("Text files (*.txt);;CSV files (*.csv);;All files (*.*)"));

	if (!path.isEmpty()) ui->pathEdit->setText(path);
}
