//
// Created by Yixiang Tu on 28/08/2018.
//

#ifndef CHIP_SIMULATOR_MAINWINDOW_H
#define CHIP_SIMULATOR_MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

class Display;
class ControlPanel;
class Toolkit;
class LogicHandler;

class MainWindow : public QMainWindow {

  Q_OBJECT

  friend class LogicHandler;

public:
  explicit MainWindow();

  ~MainWindow();

  //QSize sizeHint() const override {
  //  return QSize(960, 600);
  //}

protected:
  QWidget* widget = nullptr;
  Toolkit* toolkit = nullptr;
  Display* display = nullptr;
  ControlPanel* controlPanel = nullptr;
  LogicHandler* logicHandler = nullptr;

private:
  void initializeMenuBar();
};


#endif //CHIP_SIMULATOR_MAINWINDOW_H
