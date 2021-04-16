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

#ifndef TASKSDOCK_HPP
#define TASKSDOCK_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

#include "abstractdock.hpp"

#include "removejobwidget.hpp"
#include "copyjobwidget.hpp"

namespace Ui {	class TasksDock; }

class TasksDock : public AbstractDock
{

		Q_OBJECT

	private:

		Ui::TasksDock *ui;
		QList<QAction*> actions;

	public:

		explicit TasksDock(QWidget *parent = nullptr,
					    bool loadlast = false);
		virtual ~TasksDock(void) override;

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

		void onTasksRequest(const QVariantList&);

};

#endif // TASKSDOCK_HPP
