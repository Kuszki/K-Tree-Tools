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

	filterStringChanged();

	connect(ui->extEdit, &QLineEdit::textChanged,
		   this, &ImagevalWidget::filterStringChanged);
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
	auto text = ui->extEdit->text()
			  .replace(';', ',')
			  .replace(' ', ',');

	return text.split(',', Qt::SkipEmptyParts);
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
	ui->extEdit->setText(data.value("filter").toStringList().join(", "));

	return AbstractWidget::setData(data);
}

void ImagevalWidget::filterStringChanged(void)
{
	const bool ok = !getSelectedFormats().isEmpty();
	ui->extLabel->setStyleSheet(!ok ? wrongstyle : QString());

	emit onValidChanged(ok);
}
