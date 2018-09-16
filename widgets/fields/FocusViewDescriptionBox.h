//
// Created by Yixiang Tu on 01/09/2018.
//

#ifndef CHIP_SIMULATOR_FOCUSVIEWDESCRIPTIONBOX_H
#define CHIP_SIMULATOR_FOCUSVIEWDESCRIPTIONBOX_H


#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class Pipe;

class TextInput;

struct ControlPanelEvent;

class FocusViewDescriptionBox : public QWidget {

  Q_OBJECT

signals:
  void controlPanelEvent(const ControlPanelEvent&);

public slots:
  void pipeSelected(Pipe* target = nullptr);

  void updateInfo();

  void submitChanges();

public:
  explicit FocusViewDescriptionBox(QWidget* parent);

protected:
  Pipe* selectedPipe = nullptr;
  QPushButton* disabilityButton = nullptr;
  QPushButton* inputCompleteButton = nullptr;
  TextInput* widthInput = nullptr;
  TextInput* lengthInput = nullptr;
  TextInput* rateInput = nullptr;
  TextInput* concentrationInput = nullptr;


};


#endif //CHIP_SIMULATOR_FOCUSVIEWDESCRIPTIONBOX_H
