QT		+=	core gui widgets concurrent

CONFIG	+=	c++19

SOURCES += \
	aboutdialog.cpp \
	abstractdock.cpp \
	abstractwidget.cpp \
	common.cpp \
	configwizard.cpp \
	consoledock.cpp \
	copyjobwidget.cpp \
	deeprapwidget.cpp \
	deepvalwidget.cpp \
	emptyrapwidget.cpp \
	emptyvalwidget.cpp \
	generalpage.cpp \
	imagerapwidget.cpp \
	imagespage.cpp \
	imagevalwidget.cpp \
	itemsrapwidget.cpp \
	itemsvalwidget.cpp \
	main.cpp \
	mainwindow.cpp \
	objectspage.cpp \
	raportdock.cpp \
	removejobwidget.cpp \
	summarypage.cpp \
	tasksdock.cpp \
	threadworker.cpp \
	treevalwidget.cpp \
	treeviewdock.cpp \
	typesrapwidget.cpp \
	typesvalwidget.cpp \
	validatordock.cpp \
	wrapperwidget.cpp

HEADERS += \
	aboutdialog.hpp \
	abstractdock.hpp \
	abstractwidget.hpp \
	common.hpp \
	configwizard.hpp \
	consoledock.hpp \
	copyjobwidget.hpp \
	deeprapwidget.hpp \
	deepvalwidget.hpp \
	emptyrapwidget.hpp \
	emptyvalwidget.hpp \
	generalpage.hpp \
	imagerapwidget.hpp \
	imagespage.hpp \
	imagevalwidget.hpp \
	itemsrapwidget.hpp \
	itemsvalwidget.hpp \
	mainwindow.hpp \
	objectspage.hpp \
	raportdock.hpp \
	removejobwidget.hpp \
	summarypage.hpp \
	tasksdock.hpp \
	threadworker.hpp \
	treevalwidget.hpp \
	treeviewdock.hpp \
	typesrapwidget.hpp \
	typesvalwidget.hpp \
	validatordock.hpp \
	wrapperwidget.hpp

FORMS += \
	aboutdialog.ui \
	configwizard.ui \
	consoledock.ui \
	copyjobwidget.ui \
	deeprapwidget.ui \
	deepvalwidget.ui \
	emptyrapwidget.ui \
	emptyvalwidget.ui \
	generalpage.ui \
	imagerapwidget.ui \
	imagespage.ui \
	imagevalwidget.ui \
	itemsrapwidget.ui \
	itemsvalwidget.ui \
	mainwindow.ui \
	objectspage.ui \
	raportdock.ui \
	removejobwidget.ui \
	summarypage.ui \
	tasksdock.ui \
	treevalwidget.ui \
	treeviewdock.ui \
	typesrapwidget.ui \
	typesvalwidget.ui \
	validatordock.ui \
	wrapperwidget.ui

TRANSLATIONS += \
	K-Tree-Tools_pl_PL.ts

RESOURCES += \
	resources.qrc

INCLUDEPATH += \
	/usr/include/ImageMagick-6 \
	/usr/include/x86_64-linux-gnu/ImageMagick-6/

LIBS += \
	/usr/lib/x86_64-linux-gnu/libMagick++-6.Q16.so \
	/usr/lib/x86_64-linux-gnu/libMagickCore-6.Q16.so

