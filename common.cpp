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

#include "common.hpp"

Common::FORMAT Common::getFormat(int w, int h, int dpm)
{
	const auto ww = 1000.0 * (double(qMax(w, h)) / double(dpm));
	const auto hh = 1000.0 * (double(qMin(w, h)) / double(dpm));

	const auto cmp = [] (double a, double b) -> bool
	{
		return Common::numEq<double>(a, b, 0.1*b);
	};

	if (cmp(ww, 1189.0) && cmp(hh, 841.0)) return Common::FORMAT::A0;
	if (cmp(ww, 841.0) && cmp(hh, 594.0)) return Common::FORMAT::A1;
	if (cmp(ww, 594.0) && cmp(hh, 420.0)) return Common::FORMAT::A2;
	if (cmp(ww, 420.0) && cmp(hh, 297.0)) return Common::FORMAT::A3;
	if (cmp(ww, 297.0) && cmp(hh, 210.0)) return Common::FORMAT::A4;
	if (cmp(ww, 210.0) && cmp(hh, 148.0)) return Common::FORMAT::A5;

	return Common::FORMAT::UNKNOWN;
}

Common::FORMAT Common::getFormat(const QImage& img)
{
	const int dpm = (img.dotsPerMeterX() +
				  img.dotsPerMeterX()) / 2;

	return Common::getFormat(img.width(),
						img.height(),
						dpm);
}

int Common::getDPI(const QImage& img)
{
	return qRound((2.54 / 200.0) *
			    (img.dotsPerMeterX() +
				img.dotsPerMeterY()));
}

QString Common::getFormat(Common::FORMAT f)
{
	switch (f)
	{
		case Common::FORMAT::A0: return "A0";
		case Common::FORMAT::A1: return "A1";
		case Common::FORMAT::A2: return "A2";
		case Common::FORMAT::A3: return "A3";
		case Common::FORMAT::A4: return "A4";
		case Common::FORMAT::A5: return "A5";
		default:
		return QCoreApplication::translate("Common", "Unknown");
	}
}

bool Common::copyObject(const QString& src, const QString& dst)
{
	QFileInfo srcFileInfo(src);

	if (srcFileInfo.isDir())
	{
		QDir targetDir(dst);
		targetDir.cdUp();

		if (!targetDir.mkdir(QFileInfo(dst).fileName())) return false;

		QDir sourceDir(src);
		QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs |
										    QDir::Hidden | QDir::System |
										    QDir::NoDotAndDotDot);

		for (const auto& fileName : fileNames)
		{
			const QString newSrcFilePath
					= src + '/' + fileName;
			const QString newTgtFilePath
					= dst + '/' + fileName;

			if (!copyObject(newSrcFilePath, newTgtFilePath))
				return false;
		}
	}
	else if (srcFileInfo.isFile())
	{
		return QFile::copy(src, dst);
	}

	return true;
}
