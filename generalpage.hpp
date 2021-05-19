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

#ifndef GENERALPAGE_HPP
#define GENERALPAGE_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

namespace Ui {	class GeneralPage; }

class GeneralPage : public QWizardPage
{

		Q_OBJECT

	private:

		Ui::GeneralPage *ui;
		QString wrongstyle;

	public:

		explicit GeneralPage(QWidget *parent = nullptr);
		virtual ~GeneralPage(void) override;

		virtual void initializePage(void) override;
		virtual void cleanupPage(void) override;

		virtual bool isComplete(void) const override;

	private slots:

		void srcButtonClicked(void);
		void logButtonClicked(void);

		void formDataChanged(void);

};

#endif // GENERALPAGE_HPP
