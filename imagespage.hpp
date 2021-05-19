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

#ifndef IMAGESPAGE_HPP
#define IMAGESPAGE_HPP

#include <QWizardPage>

namespace Ui { class ImagesPage; }

class ImagesPage : public QWizardPage
{

		Q_OBJECT

	private:

		Ui::ImagesPage *ui;
		bool isLogPath = false;

	public:

		explicit ImagesPage(QWidget *parent = nullptr);
		virtual ~ImagesPage(void) override;

		virtual void initializePage(void) override;
		virtual void cleanupPage(void) override;

		virtual bool isComplete(void) const override;

	private slots:

		void formDataChanged(void);

};

#endif // IMAGESPAGE_HPP
