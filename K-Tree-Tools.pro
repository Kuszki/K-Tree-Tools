QT		+=	core gui widgets concurrent

CONFIG	+=	c++19

SOURCES += \
	aboutdialog.cpp \
	abstractdock.cpp \
	abstractwidget.cpp \
	common.cpp \
	consoledock.cpp \
	copyjobwidget.cpp \
	deeprapwidget.cpp \
	deepvalwidget.cpp \
	emptyrapwidget.cpp \
	emptyvalwidget.cpp \
	imagerapwidget.cpp \
	imagevalwidget.cpp \
	itemsrapwidget.cpp \
	itemsvalwidget.cpp \
	main.cpp \
	mainwindow.cpp \
	raportdock.cpp \
	removejobwidget.cpp \
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
	consoledock.hpp \
	copyjobwidget.hpp \
	deeprapwidget.hpp \
	deepvalwidget.hpp \
	emptyrapwidget.hpp \
	emptyvalwidget.hpp \
	imagerapwidget.hpp \
	imagevalwidget.hpp \
	itemsrapwidget.hpp \
	itemsvalwidget.hpp \
	mainwindow.hpp \
	raportdock.hpp \
	removejobwidget.hpp \
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
	consoledock.ui \
	copyjobwidget.ui \
	deeprapwidget.ui \
	deepvalwidget.ui \
	emptyrapwidget.ui \
	emptyvalwidget.ui \
	imagerapwidget.ui \
	imagevalwidget.ui \
	itemsrapwidget.ui \
	itemsvalwidget.ui \
	mainwindow.ui \
	raportdock.ui \
	removejobwidget.ui \
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
