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

#include "imagevalwidget.hpp"
#include "ui_imagevalwidget.h"

ImagevalWidget::ImagevalWidget(QWidget *parent, const QVariantMap& data)
	: AbstractWidget("imageval", parent, false)
	, ui(new Ui::ImagevalWidget)
{
	ui->setupUi(this); setData(data);

	auto model = new QStandardItemModel(0, 1, this);
	auto item = new QStandardItem(tr("All supported formats"));

	for (const auto& f : ImagevalWidget::getSupportedFormats())
	{
		auto format = new QStandardItem(f);

		format->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		format->setCheckState(Qt::Unchecked);
		format->setData(f);

		model->appendRow(format);
	}

	model->sort(0);
	item->setFlags(Qt::ItemIsEnabled);
	model->insertRow(0, item);

	ui->formatCombo->model()->deleteLater();
	ui->formatCombo->setModel(model);

	connect(model, &QStandardItemModel::itemChanged,
		   this, &ImagevalWidget::formatDataChanged);
}

ImagevalWidget::~ImagevalWidget(void)
{
	delete ui;
}

QVariantMap ImagevalWidget::getData(void) const
{
	auto formats = getSelectedFormats();
	auto base = AbstractWidget::getData();

	if (formats.isEmpty())
		formats = ImagevalWidget::getSupportedFormats();

	base.insert(
	{
		{ "dpi", ui->dpiSpin->value() },
		{ "quality", ui->qualSpin->value() },
		{ "filter", formats },
		{ "level", ui->levelSpin->value() }
	});

	return base;
}

bool ImagevalWidget::validateData(const QVariantMap& data) const
{
	return AbstractWidget::validateData(data) &&
			data.value("dpi").toInt() >= ui->dpiSpin->minimum() &&
			data.value("dpi").toInt() <= ui->dpiSpin->maximum() &&
			data.value("quality").toInt() >= ui->qualSpin->minimum() &&
			data.value("quality").toInt() <= ui->qualSpin->maximum() &&
			data.value("level").toInt() >= ui->levelSpin->minimum() &&
			data.value("level").toInt() <= ui->levelSpin->maximum();
}

QString ImagevalWidget::getDescriptionString(void) const
{
	const auto formats = getSelectedFormats().join(", ");
	const auto lvl = ui->levelSpin->value();

	return tr("%5 (level: %1, dpi: %2, quality: %3, formats: '%4')")
			.arg(lvl == -1 ? tr("Any") : QString::number(lvl))
			.arg(ui->dpiSpin->value())
			.arg(ui->qualSpin->value())
			.arg(formats.isEmpty() ? tr("All supported formats") : formats)
			.arg(getJobnameString());
}

QString ImagevalWidget::getJobnameString(void) const
{
	return tr("Checking image parameters");
}

QStringList ImagevalWidget::getSelectedFormats(void) const
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

QStringList ImagevalWidget::getSupportedFormats(void)
{
	QStringList formats;

	for (const auto& f : QImageReader::supportedImageFormats())
		formats.append(QString::fromUtf8(f));

	return formats;
}

bool ImagevalWidget::setData(const QVariantMap& data, bool force)
{
	if (!force && (data.isEmpty() || !validateData(data))) return false;

	ui->dpiSpin->setValue(data.value("dpi").toInt());
	ui->qualSpin->setValue(data.value("quality").toInt());

	auto M = dynamic_cast<QStandardItemModel*>(ui->formatCombo->model());
	const auto list = data.value("filter").toStringList();

	for (int i = 1; i < M->rowCount(); ++i)
	{
		const bool ok = list.contains(M->item(i)->data().toString());
		M->item(i)->setCheckState(ok ? Qt::Checked : Qt::Unchecked);
	}

	return AbstractWidget::setData(data);
}

void ImagevalWidget::formatDataChanged(QStandardItem* item)
{
	auto M = dynamic_cast<QStandardItemModel*>(ui->formatCombo->model());
	if (item == M->item(0)) return;

	const auto all = getSupportedFormats();
	const auto list = getSelectedFormats();

	const bool same = all.toSet() == list.toSet();

	if (!list.isEmpty() && !same) M->item(0)->setText(list.join(", "));
	else M->item(0)->setText(tr("All supported formats"));
}
