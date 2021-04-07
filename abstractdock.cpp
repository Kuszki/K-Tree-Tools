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

#include "abstractdock.hpp"

AbstractDock::AbstractDock(const QString name, QWidget* parent)
	: QDockWidget(parent)
	, dockname(name)
{
	objectsFilter = tr("All files (*.*)");
	objectFilter = tr("All files (*.*)");
}

AbstractDock::~AbstractDock(void)
{
	if (autosave) saveInSettings();
}

WrapperWidget* AbstractDock::wrappWidget(AbstractWidget* widget)
{
	return new WrapperWidget(widget, this, objectFilter);
}

AbstractWidget* AbstractDock::importWidget(const QString& path)
{
	QFile file(path);

	if (!file.open(QFile::ReadOnly)) return nullptr;

	QDataStream stream(&file);
	QVariantMap data;

	stream >> data;
	file.close();

	const QString cl = data.value("class").toString();
	AbstractWidget* widget = createWidget(cl);

	if (!widget) return nullptr;
	else widget->loadData(path);

	return widget;
}

AbstractWidget* AbstractDock::readWidget(void)
{
	const auto path = getOpenObjectFileName();

	AbstractWidget* widget = nullptr;

	if (path.isEmpty()) return nullptr;
	else widget = importWidget(path);

	if (!widget) showOpenObjectError();

	return widget;
}

bool AbstractDock::appendWidget(WrapperWidget* widget)
{
	const QString cn = widget->widget()->getClassname();

	if (widget->widget()->isSingleton())
		for (const auto w : widgets)
		{
			const auto aw = dynamic_cast<WrapperWidget*>(w);
			if (aw && aw->widget()->getClassname() == cn)
			{
				return false;
			}
		}

	connect(widget, &WrapperWidget::onUserDeleted,
		   this, &AbstractDock::widgetDeleted);

	widgets.append(widget);

	return true;
}

QString AbstractDock::getOpenObjectFileName(void)
{
	return QFileDialog::getOpenFileName(this,
			tr("Load object from file"),
			QString(),
			objectFilter);
}

void AbstractDock::showOpenObjectError(void)
{
	QMessageBox::critical(this, tr("Error"),
			tr("Provided data is not valid"));
}

QString AbstractDock::getOpenGroupFileName(void)
{
	return QFileDialog::getOpenFileName(this,
			tr("Load objects from file"),
			QString(),
			objectsFilter);
}

QString AbstractDock::getSaveGroupFileName(void)
{
	return QFileDialog::getSaveFileName(this,
			tr("Save objects to file"),
			QString(),
			objectsFilter);
}

void AbstractDock::showOpenGroupError(void)
{
	QMessageBox::critical(this, tr("Error"),
			tr("Provided data is not valid"));
}

void AbstractDock::showSaveGroupError(void)
{
	QMessageBox::critical(this, tr("Error"),
					  tr("Error saving objects data"));
}

void AbstractDock::saveInSettings(void)
{
	QSettings Settings("K-OSP", "Walidator");

	Settings.beginGroup("Session");
	Settings.setValue(dockname, getValues());
	Settings.endGroup();
}

void AbstractDock::restoreFromSettings(void)
{
	QSettings Settings("K-OSP", "Walidator");

	Settings.beginGroup("Session");
	setValues(Settings.value(dockname).toList());
	Settings.endGroup();
}

void AbstractDock::clearWidgets(void)
{
	while (!widgets.isEmpty())
	{
		widgets.takeLast()->deleteLater();
	}
}

QVariantList AbstractDock::getValues(bool enabled, bool valid) const
{
	QVariantList data;

	for (auto w : widgets)
	{
		auto aw = dynamic_cast<WrapperWidget*>(w);
		if (aw)
		{
			const bool enok = !enabled || aw->isChecked();
			const bool vok = !valid || aw->isValid();

			if (enok && vok) data.append(aw->getData());
		}
	}

	return data;
}

bool AbstractDock::setValues(const QVariantList& data, bool wipe)
{
	if (data.isEmpty()) return false;
	else if (wipe) clearWidgets();

	bool ok = true;

	for (const auto& d : data)
	{
		const auto map = d.toMap();

		const QString cl = map.value("class").toString();
		AbstractWidget* widget = createWidget(cl);

		if (!widget) ok = false;
		else
		{
			auto wrapp = wrappWidget(widget);

			widget->setData(map, true);
			appendWidget(wrapp);
		}
	}

	return ok;
}

bool AbstractDock::isAutosaveEnabled(void) const
{
	return autosave;
}

void AbstractDock::widgetDeleted(void)
{
	widgets.removeAll(dynamic_cast<QWidget*>(sender()));
}

bool AbstractDock::appendDataDialog(void)
{
	return loadDataDialog(false);
}

bool AbstractDock::replaceDataDialog(void)
{
	return loadDataDialog(true);
}

bool AbstractDock::loadDataDialog(bool wipe)
{
	const QString path = getOpenGroupFileName();
	if (path.isEmpty()) return false;
	else return loadData(path, wipe);
}

bool AbstractDock::saveDataDialog(void)
{
	const QString path = getSaveGroupFileName();

	if (path.isEmpty()) return false;
	else return saveData(path);
}

bool AbstractDock::appendData(const QString& path)
{
	return loadData(path, false);
}

bool AbstractDock::replaceData(const QString& path)
{
	return loadData(path, true);
}

bool AbstractDock::loadData(const QString& path, bool wipe)
{
	QFile file(path);

	if (!file.open(QFile::ReadOnly)) return false;

	QDataStream stream(&file);
	QVariantList data;

	stream >> data;

	return setValues(data, wipe);
}

bool AbstractDock::saveData(const QString& path) const
{
	QFile file(path);

	if (!file.open(QFile::WriteOnly)) return false;

	QDataStream stream(&file);

	stream << getValues();

	return true;
}

void AbstractDock::setAutosave(bool enabled)
{
	autosave = enabled;
}
