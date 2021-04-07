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

#ifndef ABSTRACTDOCK_HPP
#define ABSTRACTDOCK_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

#include "abstractwidget.hpp"
#include "wrapperwidget.hpp"

class AbstractDock : public QDockWidget
{

		Q_OBJECT

	protected:

		QWidgetList widgets;

		QString objectFilter;
		QString objectsFilter;

		const QString dockname;

		bool autosave = false;

	public:

		explicit AbstractDock(const QString name,
						  QWidget* parent = nullptr);
		virtual ~AbstractDock(void) override;

		QVariantList getValues(bool enabled = false,
						   bool valid = false) const;

		bool setValues(const QVariantList& data,
					bool wipe = false);

		bool isAutosaveEnabled(void) const;

		virtual void clearWidgets(void);

	protected:

		virtual WrapperWidget* wrappWidget(AbstractWidget* widget);

		virtual AbstractWidget* importWidget(const QString& path);
		virtual AbstractWidget* readWidget(void);

		virtual AbstractWidget* createWidget(const QString& name) = 0;

		virtual bool appendWidget(WrapperWidget* widget);

		virtual QString getOpenObjectFileName(void);
		virtual void showOpenObjectError(void);

		virtual QString getOpenGroupFileName(void);
		virtual QString getSaveGroupFileName(void);

		virtual void showOpenGroupError(void);
		virtual void showSaveGroupError(void);

		virtual void saveInSettings(void);
		virtual void restoreFromSettings(void);

	protected slots:

		virtual void widgetDeleted(void);

	public slots:

		virtual bool appendDataDialog(void);
		virtual bool replaceDataDialog(void);

		virtual bool loadDataDialog(bool wipe);
		virtual bool saveDataDialog(void);

		virtual bool appendData(const QString& path);
		virtual bool replaceData(const QString& path);

		virtual bool loadData(const QString& path, bool wipe);
		virtual bool saveData(const QString& path) const;

		virtual void setAutosave(bool enabled);

};

#endif // ABSTRACTDOCK_HPP
