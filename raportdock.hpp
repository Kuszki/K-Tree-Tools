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

#ifndef RAPORTDOCK_HPP
#define RAPORTDOCK_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

#include "abstractdock.hpp"

#include "wrapperwidget.hpp"
#include "deeprapwidget.hpp"
#include "emptyrapwidget.hpp"
#include "imagerapwidget.hpp"
#include "itemsrapwidget.hpp"
#include "typesrapwidget.hpp"

namespace Ui {	class RaportDock; }

class RaportDock : public AbstractDock
{

		Q_OBJECT

	private:

		Ui::RaportDock *ui;
		QList<QAction*> actions;

	public:

		explicit RaportDock(QWidget *parent = nullptr,
						bool loadlast = false);
		virtual ~RaportDock(void) override;

	protected:

		virtual AbstractWidget* createWidget(const QString& name) override;
		virtual AbstractWidget* createWidget(int id);

		virtual bool appendWidget(WrapperWidget* widget) override;

	protected slots:

		virtual void widgetDeleted(void) override;

		virtual void refreshStatus(void);

	private slots:

		void addActionClicked(void);
		void runButtonClicked(void);

	signals:

		void onRaportRequest(const QVariantList&);

};

#endif // RAPORTDOCK_HPP
