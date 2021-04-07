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

#include "consoledock.hpp"
#include "ui_consoledock.h"

ConsoleDock::ConsoleDock(QWidget *parent)
	: QDockWidget(parent)
	, ui(new Ui::ConsoleDock)
{
	ui->setupUi(this);

	connect(ui->clearButton, &QToolButton::clicked,
		   this, &ConsoleDock::clearButtonClicked);

	connect(ui->saveButton, &QToolButton::clicked,
		   this, &ConsoleDock::saveButtonClicked);

	connect(ui->searchEdit, &QLineEdit::textChanged,
		   this, &ConsoleDock::searchTextChanged);
}

ConsoleDock::~ConsoleDock(void)
{
	delete ui;
}

void ConsoleDock::showMessages(const QStringList& list)
{
	if (list.isEmpty()) lines.clear();
	else lines = list;

	const auto f = ui->searchEdit->text().trimmed();
	const auto t = lines.filter(f, Qt::CaseInsensitive);

	ui->textBrowser->setText(t.join('\n'));
	ui->saveButton->setEnabled(!t.isEmpty());
	ui->clearButton->setEnabled(!lines.isEmpty());
}

void ConsoleDock::appendMessages(const QStringList& list)
{
	if (list.isEmpty()) return;
	else lines.append(list);

	const auto f = ui->searchEdit->text().trimmed();
	const auto t = lines.filter(f, Qt::CaseInsensitive);

	ui->textBrowser->setText(t.join('\n'));
	ui->saveButton->setEnabled(!t.isEmpty());
	ui->clearButton->setEnabled(!lines.isEmpty());
}

void ConsoleDock::searchTextChanged(const QString& text)
{
	const auto t = lines.filter(text.trimmed(),
						   Qt::CaseInsensitive);

	ui->textBrowser->setText(t.join('\n'));
	ui->saveButton->setEnabled(!t.isEmpty());
}

void ConsoleDock::saveButtonClicked(void)
{
	const auto path = QFileDialog::getSaveFileName(this,
		tr("Select file to save logs"), QString(),
		tr("Text files (*.txt);;All files (*.*)"));

	if (!path.isEmpty())
	{
		QFile file(path);
		QTextStream s(&file);

		if (file.open(QFile::WriteOnly | QFile::Text))
		{
			for (const auto& l : lines)
				s << l << Qt::endl;
		}
		else QMessageBox::warning(this, tr("Error"),
			tr("Unable to create selected file"));
	}
}

void ConsoleDock::clearButtonClicked(void)
{
	ui->textBrowser->clear();
	lines.clear();

	ui->saveButton->setEnabled(false);
	ui->clearButton->setEnabled(false);
}
