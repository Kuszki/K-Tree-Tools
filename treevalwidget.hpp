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

#ifndef TREEVALWIDGET_HPP
#define TREEVALWIDGET_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

#include "abstractwidget.hpp"

namespace Ui {	class TreevalWidget; }

class TreevalWidget : public AbstractWidget
{

		Q_OBJECT

	private:

		Ui::TreevalWidget *ui;

	public:

		explicit TreevalWidget(QWidget *parent = nullptr,
						   const QVariantMap& data = QVariantMap());
		virtual ~TreevalWidget(void) override;

		virtual QVariantMap getData(void) const override;

		virtual bool validateData(const QVariantMap& data) const override;

		virtual QString getDescriptionString(void) const override;
		virtual QString getJobnameString(void) const override;

	public slots:

		virtual bool setData(const QVariantMap& data,
						 bool force = false) override;

	private slots:

		void pathStringChanged(const QString& path);

		void openButtonClicked(void);

};

#endif // TREEVALWIDGET_HPP
