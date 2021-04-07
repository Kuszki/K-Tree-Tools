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

#ifndef TREEVIEWDOCK_HPP
#define TREEVIEWDOCK_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

namespace Ui {	class TreeviewDock; }

class TreeviewDock : public QDockWidget
{

		Q_OBJECT

	private:

		Ui::TreeviewDock *ui;

		QFileSystemModel* model;

	public:

		explicit TreeviewDock(QWidget *parent = nullptr);
		virtual ~TreeviewDock(void) override;

	public slots:

		void selectRoot(const QString& path);

	private slots:

		void fileButtonClicked(void);
		void dirButtonClicked(void);

};

#endif // TREEVIEWDOCK_HPP
