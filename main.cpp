#include <QApplication>
#include "widgets/fields/MainWindow.h"

int main(int argc, char* argv[]) {

  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}
