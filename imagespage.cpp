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

#include "imagespage.hpp"
#include "ui_imagespage.h"

ImagesPage::ImagesPage(QWidget *parent)
	: QWizardPage(parent)
	, ui(new Ui::ImagesPage)
{
	ui->setupUi(this); formDataChanged();

	registerField("chjpeg", ui->jpegCheck);
	registerField("chtiff", ui->tiffCheck);
	registerField("chpdf", ui->pdfCheck);
	registerField("chcorr", ui->corruptedCheck);
	registerField("mkrapp", ui->summaryCheck);
	registerField("reqres", ui->resCombo);
	registerField("reqqual", ui->qualCombo);

	connect(ui->jpegCheck, &QCheckBox::toggled,
		   this, &ImagesPage::formDataChanged);

	connect(ui->tiffCheck, &QCheckBox::toggled,
		   this, &ImagesPage::formDataChanged);

	connect(ui->pdfCheck, &QCheckBox::toggled,
		   this, &ImagesPage::formDataChanged);

	connect(ui->corruptedCheck, &QCheckBox::toggled,
		   this, &ImagesPage::formDataChanged);

	connect(ui->summaryCheck, &QCheckBox::toggled,
		   this, &ImagesPage::formDataChanged);

	connect(ui->qualCombo, &QComboBox::currentTextChanged,
		   this, &ImagesPage::formDataChanged);

	connect(ui->resCombo, &QComboBox::currentTextChanged,
		   this, &ImagesPage::formDataChanged);
}

ImagesPage::~ImagesPage(void)
{
	delete ui;
}

void ImagesPage::initializePage(void)
{
	isLogPath = !field("logpath").toString().isEmpty();

	ui->summaryCheck->setEnabled(isLogPath &&
				ui->summaryCheck->isEnabled());
}

void ImagesPage::cleanupPage(void) {}

bool ImagesPage::isComplete(void) const
{
	const bool anyCheck = ui->jpegCheck->isChecked() ||
					  ui->tiffCheck->isChecked() ||
					  ui->pdfCheck->isChecked();

	const bool anyJob = ui->qualCombo->currentIndex() ||
					ui->resCombo->currentIndex() ||
					ui->corruptedCheck->isChecked() ||
					(ui->summaryCheck->isChecked() &&
					 ui->summaryCheck->isEnabled());

	return !anyCheck || (anyCheck && anyJob);
}

void ImagesPage::formDataChanged(void)
{
	const bool anyCheck = ui->jpegCheck->isChecked() ||
					  ui->tiffCheck->isChecked() ||
					  ui->pdfCheck->isChecked();

	ui->qualCombo->setEnabled(anyCheck);
	ui->resCombo->setEnabled(anyCheck);
	ui->corruptedCheck->setEnabled(anyCheck);
	ui->summaryCheck->setEnabled(anyCheck && isLogPath);

	emit completeChanged();
}
