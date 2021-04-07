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

#ifndef COMMON_HPP
#define COMMON_HPP

#include <QtCore>
#include <QtGui>

namespace Common
{
	enum class FORMAT
	{
		UNKNOWN,

		A0,
		A1,
		A2,
		A3,
		A4,
		A5
	};

	struct NODE
	{
		QFileInfo info;
		int level = 0;

		QSharedPointer<NODE> parent;
	};

	using NODEPTR = QSharedPointer<NODE>;
	using NODELIST = QList<NODEPTR>;
	using NODESET = QSet<NODEPTR>;

	FORMAT getFormat(int w, int h, int dpm);
	FORMAT getFormat(const QImage& img);

	QString getFormat(FORMAT f);

	int getDPI(const QImage& img);

	template<class Number>
	bool numEq(const Number& a,
			 const Number& b,
			 const Number& d);
}

#endif // COMMON_HPP
