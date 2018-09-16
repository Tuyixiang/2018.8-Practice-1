//
// Created by Yixiang Tu on 30/08/2018.
//

#ifndef CHIP_SIMULATOR_LOGICHANDLER_H
#define CHIP_SIMULATOR_LOGICHANDLER_H
#include <QObject>
#include <QDebug>
#include "EnumTypes.h"

class Pipe;

struct ControlPanelEvent;

class MainWindow;

class LogicHandler : public QObject {

  Q_OBJECT

public:
  explicit LogicHandler(QObject* parent);

signals:
  void requireDisplayRedraw();

  void requireChipReexamine();

  void reset();

  void setFocus(Pipe*);

  void setFocusViewTool(Tools);

  void turnOffHighlight(Pipe*);

  void turnOnHighlight(Pipe*);

  void switchDisability(Pipe*);

public slots:
  void toolSwitch(Tools t);

  void pipePressed(Pipe* target);

  void pipeMouseMove(Pipe* target);

  void pipeRightClicked(Pipe* target);

  void defaultPressed();

  void defaultMouseMove();

  void released();

  void controlPanelEvent(const ControlPanelEvent& ev);

protected:
  bool mouseDown = false;

  MainWindow* const mainWindow;

  Tools activeTool = Tools::select;

  void resetPipes() {}

  Pipe* currentFocus = nullptr;

};


#endif //CHIP_SIMULATOR_LOGICHANDLER_H
