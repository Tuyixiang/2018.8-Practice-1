//
// Created by Yixiang Tu on 01/09/2018.
//

#ifndef CHIP_SIMULATOR_TEXTINPUT_H
#define CHIP_SIMULATOR_TEXTINPUT_H


#include <QLineEdit>

class TextInput : public QLineEdit {
  Q_OBJECT

signals:
  void inputComplete();

public:
  TextInput(QWidget* parent);

protected:
  void keyPressEvent(QKeyEvent* ev) override;

  QSize sizeHint() const override {
    return QSize(64, 21);
  }
};


#endif //CHIP_SIMULATOR_TEXTINPUT_H
