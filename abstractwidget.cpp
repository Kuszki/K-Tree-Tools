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

#include "abstractwidget.hpp"

AbstractWidget::AbstractWidget(const QString name, QWidget* parent, bool single)
	: QWidget(parent)
	, classname(name)
	, singleton(single)
{
	wrongstyle = "text-decoration: underline wavy red;";
}

AbstractWidget::~AbstractWidget(void) {}

QVariantMap AbstractWidget::getData(void) const
{
	return
	{
		{ "class", getClassname() },
		{ "active", isActive() },
		{ "brief", getDescriptionString() },
		{ "name", getJobnameString() }
	};
}

bool AbstractWidget::validateData(const QVariantMap& data) const
{
	return data.value("class").toString() == getClassname();
}

bool AbstractWidget::isActive(void) const
{
	return enabled;
}

void AbstractWidget::setActive(bool active)
{
	if (enabled == active) return;

	emit onActiveChanged(enabled = active);
}

bool AbstractWidget::setData(const QVariantMap& data, bool force)
{
	setActive(data.value("active", true).toBool()); return true;
}

const QString& AbstractWidget::getClassname(void) const
{
	return classname;
}

bool AbstractWidget::isValid(void) const
{
	return validateData(getData());
}

bool AbstractWidget::isSingleton(void) const
{
	return singleton;
}

bool AbstractWidget::loadData(const QString& path)
{
	QFile file(path);

	if (!file.open(QFile::ReadOnly)) return false;

	QDataStream stream(&file);
	QVariantMap data;

	stream >> data;
	setData(data, true);

	return true;
}

bool AbstractWidget::saveData(const QString& path) const
{
	QFile file(path);

	if (!file.open(QFile::WriteOnly)) return false;

	QDataStream stream(&file);

	stream << getData();

	return true;
}
