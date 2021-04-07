#include "mainwindow.hpp"

#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>

#include "common.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	qRegisterMetaType<Common::NODELIST>("Common::NOODELIST");

//	QIcon::setThemeSearchPaths({ "icons" });
//	QIcon::setThemeName("gnome");

	a.setApplicationName("K-Tree-Tools");
	a.setOrganizationName("Łukasz \"Kuszki\" Dróżdż");
	a.setOrganizationDomain("https://github.com/Kuszki");
	a.setApplicationVersion("1.0");

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtTranslator);

	QTranslator baseTranslator;
	baseTranslator.load("qtbase_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&baseTranslator);

	QTranslator appTranslator;
	appTranslator.load("K-Tree-Tools_" + QLocale::system().name());
	a.installTranslator(&appTranslator);

	MainWindow w;
	w.show();

	return a.exec();
}
