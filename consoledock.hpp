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

#ifndef CONSOLEDOCK_HPP
#define CONSOLEDOCK_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

namespace Ui {	class ConsoleDock; }

class ConsoleDock : public QDockWidget
{

		Q_OBJECT

	private:

		Ui::ConsoleDock *ui;
		QStringList lines;

	public:

		explicit ConsoleDock(QWidget *parent = nullptr);
		virtual ~ConsoleDock(void) override;

	public slots:

		void showMessages(const QStringList& list);
		void appendMessages(const QStringList& list);

	private slots:

		void searchTextChanged(const QString& text);

		void saveButtonClicked(void);
		void clearButtonClicked(void);

};

#endif // CONSOLEDOCK_HPP
