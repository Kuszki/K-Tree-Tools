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

#include "imagerapwidget.hpp"
#include "ui_imagerapwidget.h"

ImagerapWidget::ImagerapWidget(QWidget *parent, const QVariantMap& data) :
	AbstractWidget("imagerap", parent, false),
	ui(new Ui::ImagerapWidget)
{
	ui->setupUi(this); setData(data); int index(0);

	auto fmodel = new QStandardItemModel(0, 1, this);
	auto fitem = new QStandardItem(tr("All supported formats"));

	auto amodel = new QStandardItemModel(0, 1, this);
	auto aitem = new QStandardItem(tr("All raports"));

	const QStringList actions =
	{
		tr("Count selected files"),
		tr("Count sheets by size"),
		tr("Count sheets by resolution")
	};

	for (const auto& f : ImagerapWidget::getSupportedFormats())
	{
		auto format = new QStandardItem(f);

		format->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		format->setCheckState(Qt::Unchecked);
		format->setData(f);

		fmodel->appendRow(format);
	}

	for (const auto& f : actions)
	{
		auto format = new QStandardItem(f);

		format->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		format->setCheckState(Qt::Unchecked);
		format->setData(1 << index++);

		amodel->appendRow(format);
	}

	fmodel->sort(0);
	fitem->setFlags(Qt::ItemIsEnabled);
	fmodel->insertRow(0, fitem);

	amodel->sort(0);
	aitem->setFlags(Qt::ItemIsEnabled);
	amodel->insertRow(0, aitem);

	ui->formatCombo->model()->deleteLater();
	ui->formatCombo->setModel(fmodel);

	ui->actionCombo->model()->deleteLater();
	ui->actionCombo->setModel(amodel);

	connect(fmodel, &QStandardItemModel::itemChanged,
		   this, &ImagerapWidget::formatDataChanged);

	connect(amodel, &QStandardItemModel::itemChanged,
		   this, &ImagerapWidget::actionDataChanged);
}

ImagerapWidget::~ImagerapWidget(void)
{
	delete ui;
}

QVariantMap ImagerapWidget::getData(void) const
{
	auto action = getSelectedActions();

	auto formats = getSelectedFormats();
	auto base = AbstractWidget::getData();

	if (formats.isEmpty())
		formats = ImagerapWidget::getSupportedFormats();

	base.insert(
	{
		{ "actions", action },
		{ "filter", formats },
		{ "level", ui->levelSpin->value() }
	});

	return base;
}

bool ImagerapWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("level").toInt() >= ui->levelSpin->minimum() &&
			data.value("level").toInt() <= ui->levelSpin->maximum();
}

QString ImagerapWidget::getDescriptionString(void) const
{
	const auto formats = getSelectedFormats().join(", ");
	const auto lvl = ui->levelSpin->value();

	return tr("%3 (level: %1, formats: '%2')")
			.arg(lvl == -1 ? tr("Any") : QString::number(lvl))
			.arg(formats.isEmpty() ? tr("All supported formats") : formats)
			.arg(getJobnameString());
}

QString ImagerapWidget::getJobnameString(void) const
{
	return tr("Counting image parameters");
}

int ImagerapWidget::getSelectedActions(void) const
{
	auto M = dynamic_cast<QStandardItemModel*>(ui->actionCombo->model());

	int checked(0);

	for (int i = 1; i < M->rowCount(); ++i)
		if (M->item(i)->checkState() == Qt::Checked)
		{
			checked |= M->item(i)->data().toInt();
		}

	return checked;
}

QStringList ImagerapWidget::getSelectedFormats(void) const
{
	auto M = dynamic_cast<QStandardItemModel*>(ui->formatCombo->model());

	QStringList checked;

	for (int i = 1; i < M->rowCount(); ++i)
		if (M->item(i)->checkState() == Qt::Checked)
		{
			checked << M->item(i)->data().toString();
		}

	return checked;
}

QStringList ImagerapWidget::getSupportedFormats(void)
{
	QStringList formats;

	for (const auto& f : QImageReader::supportedImageFormats())
		formats.append(QString::fromUtf8(f));

	return formats;
}

bool ImagerapWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	auto M1 = dynamic_cast<QStandardItemModel*>(ui->formatCombo->model());
	const auto list1 = data.value("filter").toStringList();

	for (int i = 1; i < M1->rowCount(); ++i)
	{
		const bool ok = list1.contains(M1->item(i)->data().toString());
		M1->item(i)->setCheckState(ok ? Qt::Checked : Qt::Unchecked);
	}

	auto M2 = dynamic_cast<QStandardItemModel*>(ui->formatCombo->model());
	const auto list = data.value("actions").toInt();

	for (int i = 1; i < M2->rowCount(); ++i)
	{
		const bool ok = list & (1 << (i-1));
		M2->item(i)->setCheckState(ok ? Qt::Checked : Qt::Unchecked);
	}

	return AbstractWidget::setData(data);
}

void ImagerapWidget::actionDataChanged(QStandardItem* item)
{
	auto M = dynamic_cast<QStandardItemModel*>(ui->actionCombo->model());
	if (item == M->item(0)) return;

	const auto actions = getSelectedActions();
	const bool same = actions == 0b111;
	const bool none = actions == 0b000;

	if (!same && !none) M->item(0)->setText("Selected raports");
	else M->item(0)->setText(tr("All raports"));
}

void ImagerapWidget::formatDataChanged(QStandardItem* item)
{
	auto M = dynamic_cast<QStandardItemModel*>(ui->formatCombo->model());
	if (item == M->item(0)) return;

	const auto all = getSupportedFormats();
	const auto list = getSelectedFormats();

	const bool same = all.toSet() == list.toSet();

	if (!list.isEmpty() && !same) M->item(0)->setText(list.join(", "));
	else M->item(0)->setText(tr("All supported formats"));
}
