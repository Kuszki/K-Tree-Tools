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

#ifndef WRAPPERWIDGET_HPP
#define WRAPPERWIDGET_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

#include "abstractwidget.hpp"

namespace Ui {	class WrapperWidget; }

class WrapperWidget : public QWidget
{

		Q_OBJECT

	private:

		Ui::WrapperWidget *ui;
		AbstractWidget* wrapped;

		QString fileFilter;

	public:

		explicit WrapperWidget(AbstractWidget *widget,
						   QWidget *parent = nullptr,
						   const QString& filter = QString());
		virtual ~WrapperWidget(void) override;

		const AbstractWidget* widget(void) const;
		AbstractWidget* widget(void);

		bool isChecked(void) const;
		bool isValid(void) const;

		QVariantMap getData(void) const;

	public slots:

		void setChecked(bool checked);

	private slots:

		void valStatusChanged(bool ok);

		void saveButtonClicked(void);
		void runButtonClicked(void);

	signals:

		void onRunRequest(const QVariant&);

		void onCheckChanged(bool);
		void onValidChanged(bool);
		void onUserDeleted(void);

};

#endif // WRAPPERWIDGET_HPP
