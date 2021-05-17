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

#ifndef TYPESRAPWIDGET_HPP
#define TYPESRAPWIDGET_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

#include "abstractwidget.hpp"

namespace Ui {	class TypesrapWidget; }

class TypesrapWidget : public AbstractWidget
{

		Q_OBJECT

	private:

		Ui::TypesrapWidget *ui;

	public:

		explicit TypesrapWidget(QWidget *parent = nullptr,
						    const QVariantMap& data = QVariantMap());
		virtual ~TypesrapWidget(void) override;

		virtual QVariantMap getData(void) const override;

		virtual bool validateData(const QVariantMap& data) const override;

		virtual QString getDescriptionString(void) const override;
		virtual QString getJobnameString(void) const override;

		QStringList getSelectedExtensions(void) const;

	public slots:

		virtual bool setData(const QVariantMap& data,
						 bool force = false) override;

	private slots:

		void filterStringChanged(void);

};

#endif // TYPESRAPWIDGET_HPP
