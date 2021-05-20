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

#include "configwizard.hpp"
#include "ui_configwizard.h"

ConfigWizard::ConfigWizard(QWidget *parent)
	: QWizard(parent)
	, ui(new Ui::ConfigWizard)
{
	ui->setupUi(this);
}

ConfigWizard::~ConfigWizard(void)
{
	delete ui;
}

QVariantMap ConfigWizard::getTypesvalData(void) const
{
	QStringList objExts = field("objext").toString()
					  .replace(';', ',')
					  .replace(' ', ',')
					  .replace('\n', ',')
					  .split(',', Qt::SkipEmptyParts);

	if (objExts.isEmpty()) return QVariantMap();

	if (field("chjpeg").toBool()) objExts.append({ "jpg", "jpeg" });
	if (field("chtiff").toBool()) objExts.append({ "tif", "tiff" });
	if (field("chpdf").toBool()) objExts.append("pdf");

	return
	{
		{ "class", "typesval" },
		{ "action", 0 },
		{ "filter", objExts },
		{ "level", -1 }
	};
}

QVariantMap ConfigWizard::getImagesvalData(void) const
{
	const QMap<int, int> dpiMap =
	{
		{1, 200}, {2, 300}, {3, 400}, {4, 600}
	};

	const QMap<int, QPair<int, int>> qualMap =
	{
		{ 1, {50, 70} }, { 2, {70, 100} }
	};

	QStringList imgExts;

	if (field("chjpeg").toBool()) imgExts.append({ "jpg", "jpeg" });
	if (field("chtiff").toBool()) imgExts.append({ "tif", "tiff" });
	if (field("chpdf").toBool()) imgExts.append("pdf");

	const int reqres = dpiMap.value(field("reqres").toInt(), 0);
	const QPair<int, int> reqqual = qualMap.value(field("reqqual").toInt(), {0, 100});

	if (imgExts.isEmpty() || !(reqres || reqqual.first)) return QVariantMap();
	else return
	{
		{ "class", "imageval" },
		{ "dpi", reqres },
		{ "minquality", reqqual.first },
		{ "maxquality", reqqual.second },
		{ "filter", imgExts },
		{ "level", -1 }
	};
}

QVariantMap ConfigWizard::getEmptyvalData(void) const
{
	if (!field("chempt").toBool()) return QVariantMap();
	else return
	{
		{ "class", "emptyval" },
		{ "action", 0 },
		{ "objects", 2 },
		{ "level", -1 }
	};
}

QVariantMap ConfigWizard::getImagesrapData(void) const
{
	if (!field("mkrapp").toBool()) return QVariantMap();

	QStringList imgExts;

	if (field("chjpeg").toBool()) imgExts.append({ "jpg", "jpeg" });
	if (field("chtiff").toBool()) imgExts.append({ "tif", "tiff" });
	if (field("chpdf").toBool()) imgExts.append("pdf");

	if (imgExts.isEmpty()) return QVariantMap();
	else return
	{
		{ "class", "imagerap" },
		{ "filter", imgExts },
		{ "actions", 1 },
		{ "level", -1 }
	};
}

void ConfigWizard::accept(void)
{
	QWizard::accept();

	QVariantMap values;

	values["root"] = field("srcpath");
	values["logs"] = field("logpath");

	values["logging"] = field("mklogs").toBool();
	values["autorun"] = field("autorun").toBool();

	QVariantList vals, tass, raps;

	const QVariantMap typesVal = getTypesvalData();
	const QVariantMap emptyVal = getEmptyvalData();
	const QVariantMap imageVal = getImagesvalData();
	const QVariantMap imageRap = getImagesrapData();

	if (!typesVal.isEmpty()) vals.append(typesVal);
	if (!emptyVal.isEmpty()) vals.append(emptyVal);
	if (!imageVal.isEmpty()) vals.append(imageVal);
	if (!imageRap.isEmpty()) raps.append(imageRap);

	values["vals"] = vals;
	values["tass"] = tass;
	values["raps"] = raps;

	emit onAccepted(values);
}
