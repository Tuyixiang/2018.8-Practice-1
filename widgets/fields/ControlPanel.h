//
// Created by Yixiang Tu on 28/08/2018.
//

#ifndef CHIP_SIMULATOR_CONTROLPANEL_H
#define CHIP_SIMULATOR_CONTROLPANEL_H

#include "QWidget"

class QPushButton;

class QGraphicsScene;

class FocusView;

class FocusViewDescriptionBox;

class GlobalSettingsBox;

class Pipe;

struct ControlPanelEvent {
  enum EventType {
    disabilitySwitch,
    pipeChange,
    randomizeMap,
    randomizePipeSize,
    clearPipes,
    calculate,
    reset,
    badInput,
    chipChange
  } type;

  Pipe* target = nullptr;
  int width = 0;
  int length = 0;
  qreal rate = 0;
  qreal concentration = 0;
  QString message;

  explicit ControlPanelEvent(EventType t) {
    type = t;
  }

  ControlPanelEvent(EventType t, QString&& m) {
    type = t;
    message = m;
  }

  explicit ControlPanelEvent(Pipe* tar, EventType t = EventType::disabilitySwitch) {
    type = t;
    target = tar;
  }

  ControlPanelEvent(Pipe* tar, int w, int l, qreal r = 0, qreal c = 0) {
    target = tar;
    type = EventType::pipeChange;
    width = w;
    length = l;
    rate = r;
    concentration = c;
  }

  ControlPanelEvent(int rows, int cols) {
    type = EventType::chipChange;
    width = rows;
    length = cols;
  }

};

class ControlPanel : public QWidget {

  friend class MainWindow;

  friend class LogicHandler;

Q_OBJECT

public:
  explicit ControlPanel(QWidget* parent);

  void setScene(QGraphicsScene* s);

signals:

  void controlPanelEvent(const ControlPanelEvent& ev);

protected:
  FocusViewDescriptionBox* focusViewDescriptionBox;
  GlobalSettingsBox* globalSettingsBox;
  FocusView* focusView;

};


#endif //CHIP_SIMULATOR_CONTROLPANEL_H
