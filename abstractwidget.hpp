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

#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

class AbstractWidget : public QWidget
{

		Q_OBJECT

	private:

		const QString classname;

		bool enabled = true;
		const bool singleton;

	protected:

		QString wrongstyle;

	public:

		explicit AbstractWidget(const QString name,
						    QWidget *parent = nullptr,
						    bool single = false);

		virtual ~AbstractWidget(void) override;

		virtual QVariantMap getData(void) const;

		virtual bool validateData(const QVariantMap& data) const;

		virtual QString getDescriptionString(void) const = 0;
		virtual QString getJobnameString(void) const = 0;

		const QString& getClassname(void) const;

		bool isValid(void) const;
		bool isSingleton(void) const;

		bool isActive(void) const;
		void setActive(bool active);

	public slots:

		virtual bool setData(const QVariantMap& data,
						 bool force = false);

		virtual bool loadData(const QString& path);
		virtual bool saveData(const QString& path) const;

	signals:

		void onActiveChanged(bool);
		void onValidChanged(bool);

};

#endif // ABSTRACTWIDGET_H
