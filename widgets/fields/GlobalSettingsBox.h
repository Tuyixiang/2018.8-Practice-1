//
// Created by Yixiang Tu on 01/09/2018.
//

#ifndef CHIP_SIMULATOR_GLOBALSETTINGSBOX_H
#define CHIP_SIMULATOR_GLOBALSETTINGSBOX_H


#include <QWidget>
#include <QSpinBox>
#include <QPushButton>

struct ControlPanelEvent;

class GlobalSettingsBox : public QWidget {

  Q_OBJECT

signals:
  void controlPanelEvent(const ControlPanelEvent&);

public slots:
  void updateSettings();

public:
  GlobalSettingsBox(QWidget* parent);

protected:
  QSpinBox* rowInput;
  QSpinBox* colInput;
  QPushButton* chipSizeButton;
};


#endif //CHIP_SIMULATOR_GLOBALSETTINGSBOX_H
