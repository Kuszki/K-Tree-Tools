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

#include "summarypage.hpp"
#include "ui_summarypage.h"

SummaryPage::SummaryPage(QWidget *parent)
	: QWizardPage(parent)
	, ui(new Ui::SummaryPage)
{
	ui->setupUi(this);
}

SummaryPage::~SummaryPage(void)
{
	delete ui;
}

void SummaryPage::initializePage(void)
{
	QStringList imgExts, lines;

	const QMap<int, int> dpiMap =
	{
		{1, 200}, {2, 300}, {3, 400}, {4, 600}
	};

	const QMap<int, QPair<int, int>> qualMap =
	{
		{ 1, {50, 70} }, { 2, {70, 100} }
	};

	const QString srcPath = field("srcpath").toString();
	const QString logPath = field("logpath").toString();

	QStringList objExts = field("objext").toString()
					  .replace(';', ',')
					  .replace(' ', ',')
					  .replace('\n', ',')
					  .split(',', Qt::SkipEmptyParts);

	const bool mkrapp = field("mkrapp").toBool() && !logPath.isEmpty();
	const bool mklogs = field("mklogs").toBool();

	const bool chempt = field("chempt").toBool();
	const bool chcorr = field("chcorr").toBool();
	const bool autorun = field("autorun").toBool();

	const int reqres = dpiMap.value(field("reqres").toInt(), 0);
	const QPair<int, int> reqqual = qualMap.value(field("reqqual").toInt(), {0, 100});

	if (field("chjpeg").toBool()) imgExts.append({ "jpg", "jpeg" });
	if (field("chtiff").toBool()) imgExts.append({ "tif", "tiff" });
	if (field("chpdf").toBool()) imgExts.append("pdf");

	if (!objExts.isEmpty()) objExts.append(imgExts);
	if (!objExts.isEmpty()) objExts.removeDuplicates();

	const bool isObjJob = !objExts.isEmpty() || chempt;
	const bool isImgJob = !imgExts.isEmpty() && (chcorr || mkrapp || reqres || reqqual.first);

	if (!(isObjJob || isImgJob))
	{
		lines.append(tr("<h3>No jobs selected</h3>"));
	}
	else
	{
		lines.append(tr("<h3>General options</h3>"));
		lines.append("<ul>");

		lines.append(tr("<li>Source directory: <a href='file://%1'>%1</a></li>").arg(srcPath));

		if (!logPath.isEmpty()) lines.append(tr("<li>Logs directory: <a href='file://%1'>%1</a></li>").arg(logPath));
		if (mklogs) lines.append(tr("<li>Enable raport creation: yes</li>"));
		if (autorun) lines.append(tr("<li>Automatic run after wizard: yes</li>"));

		lines.append("</ul>");
	}

	if (isObjJob)
	{
		lines.append(tr("<h3>Check filesystem objects</h3>"));
		lines.append("<ul>");

		if (!objExts.isEmpty()) lines.append(tr("<li>Allowed extensions: %1</li>").arg(objExts.join(", ")));
		if (chempt) lines.append(tr("<li>Empty diriectories: not allowed</li>"));

		lines.append("</ul>");
	}

	if (isImgJob)
	{
		lines.append(tr("<h3>Check images parameters</h3>"));
		lines.append("<ul>");

		lines.append(tr("<li>Checked extensions: %1</li>").arg(imgExts.join(", ")));

		if (reqres) lines.append(tr("<li>Required resolution: %1 DPI</li>").arg(reqres));
		if (reqqual.first) lines.append(tr("<li>Required quality: %1-%2%</li>").arg(reqqual.first).arg(reqqual.second));
		if (chcorr) lines.append(tr("<li>Check corrupted image files: yes</li>"));
		if (mkrapp) lines.append(tr("<li>Generate summary raport: yes</li>"));

		lines.append("</ul>");
	}

	ui->label->setText(lines.join('\n'));

	isAnyJob = isObjJob || isImgJob;
}

void SummaryPage::cleanupPage(void) {}

bool SummaryPage::isComplete(void) const
{
	return isAnyJob;
}
