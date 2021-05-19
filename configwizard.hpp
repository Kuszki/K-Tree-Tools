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

#ifndef CONFIGWIZARD_HPP
#define CONFIGWIZARD_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

namespace Ui {	class ConfigWizard; }

class ConfigWizard : public QWizard
{

		Q_OBJECT

	private:

		Ui::ConfigWizard *ui;

	public:

		explicit ConfigWizard(QWidget *parent = nullptr);
		virtual ~ConfigWizard(void) override;

		QVariantMap getTypesvalData(void) const;
		QVariantMap getImagesvalData(void) const;
		QVariantMap getEmptyvalData(void) const;
		QVariantMap getImagesrapData(void) const;

	public slots:

		virtual void accept(void) override;

	signals:

		void onAccepted(const QVariantMap&);

};

#endif // CONFIGWIZARD_HPP
