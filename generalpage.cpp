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

#include "generalpage.hpp"
#include "ui_generalpage.h"

GeneralPage::GeneralPage(QWidget *parent)
	: QWizardPage(parent)
	, ui(new Ui::GeneralPage)
{
	wrongstyle = "text-decoration: underline wavy red;";

	ui->setupUi(this); formDataChanged();

	registerField("srcpath", ui->srcEdit);
	registerField("logpath", ui->logEdit);
	registerField("mklogs", ui->logenCheck);
	registerField("autorun", ui->autorunCheck);

	connect(ui->srcButton, &QToolButton::clicked,
		   this, &GeneralPage::srcButtonClicked);

	connect(ui->logButton, &QToolButton::clicked,
		   this, &GeneralPage::logButtonClicked);

	connect(ui->srcEdit, &QLineEdit::textChanged,
		   this, &GeneralPage::formDataChanged);

	connect(ui->logEdit, &QLineEdit::textChanged,
		   this, &GeneralPage::formDataChanged);

	connect(ui->logenCheck, &QCheckBox::toggled,
		   this, &GeneralPage::formDataChanged);
}

GeneralPage::~GeneralPage(void)
{
	delete ui;
}

void GeneralPage::initializePage(void)
{
	formDataChanged();
}

void GeneralPage::cleanupPage(void) {}

bool GeneralPage::isComplete(void) const
{
	const bool srcok = !ui->srcEdit->text().isEmpty() &&
				    QDir(ui->srcEdit->text()).exists();
	const bool logok = !ui->logEdit->text().isEmpty() &&
				    QDir(ui->logEdit->text()).exists();
	const bool islog = ui->logenCheck->isChecked();

	return srcok && (logok || !islog);
}

void GeneralPage::srcButtonClicked(void)
{
	const QString path = QFileDialog::getExistingDirectory(this,
		tr("Select root directory"), ui->srcEdit->text());

	if (!path.isEmpty()) ui->srcEdit->setText(path);
}

void GeneralPage::logButtonClicked(void)
{
	const QString path = QFileDialog::getExistingDirectory(this,
		tr("Select log directory"), ui->logEdit->text());

	if (!path.isEmpty()) ui->logEdit->setText(path);
}

void GeneralPage::formDataChanged(void)
{
	const bool srcok = !ui->srcEdit->text().isEmpty() &&
				    QDir(ui->srcEdit->text()).exists();
	const bool logok = !ui->logEdit->text().isEmpty() &&
				    QDir(ui->logEdit->text()).exists();
	const bool islog = ui->logenCheck->isChecked();

	ui->srcLabel->setStyleSheet(!srcok ? wrongstyle : QString());
	ui->logLabel->setStyleSheet(islog && !logok ? wrongstyle : QString());

	emit completeChanged();
}
