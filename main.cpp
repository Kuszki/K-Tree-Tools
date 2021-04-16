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

	const QString tp = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	const QString ln = QLocale::system().name();

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + ln, tp);
	a.installTranslator(&qtTranslator);

	QTranslator baseTranslator;
	baseTranslator.load("qtbase_" + ln, tp);
	a.installTranslator(&baseTranslator);

	QTranslator appTranslator;
	appTranslator.load("K-Tree-Tools_" + ln);
	a.installTranslator(&appTranslator);

	MainWindow w;
	w.show();

	return a.exec();
}
