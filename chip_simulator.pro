QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = chip_simulator
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
        main.cpp \
        widgets/fields/MainWindow.cpp \
        widgets/fields/Display.cpp \
        widgets/fields/ControlPanel.cpp \
        Chip.cpp \
        widgets/fields/Toolkit.cpp \
        widgets/items/Pipe.cpp \
        MyMath.cpp \
        widgets/items/ToolkitButton.cpp \
        LogicHandler.cpp \
        widgets/fields/FocusView.cpp \
        widgets/items/PipeHighlight.cpp \
        widgets/items/Scene.cpp \
        widgets/fields/FocusViewDescriptionBox.cpp \
        widgets/items/TextInput.cpp \
        widgets/fields/GlobalSettingsBox.cpp \
        widgets/fields/ChipSizePopup.cpp \
        Calculation.cpp
HEADERS += \
        widgets/fields/MainWindow.h \
        widgets/fields/Display.h \
        widgets/fields/ControlPanel.h \
        Chip.h \
        widgets/fields/Toolkit.h \
        widgets/items/Pipe.h \
        widgets/items/ToolkitButton.h \
        LogicHandler.h \
        widgets/fields/FocusView.h \
        widgets/items/PipeHighlight.h \
        widgets/items/Scene.h \
        EnumTypes.h \
        widgets/fields/FocusViewDescriptionBox.h \
        widgets/items/TextInput.h \
        widgets/fields/GlobalSettingsBox.h \
        widgets/fields/ChipSizePopup.h \
        Calculation.h \
    MyMath.h

RESOURCES += \
    src.qrc
