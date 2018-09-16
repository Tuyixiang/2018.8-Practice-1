//
// Created by Yixiang Tu on 01/09/2018.
//

#include "TextInput.h"
#include <QKeyEvent>
#include <QDebug>

TextInput::TextInput(QWidget* parent) : QLineEdit(parent) {

}

void TextInput::keyPressEvent(QKeyEvent* ev) {
  switch (ev->key()) {
    case Qt::Key_Return: {
      emit inputComplete();
      break;
    }
    default: {
      QLineEdit::keyPressEvent(ev);
    }
  }
}
