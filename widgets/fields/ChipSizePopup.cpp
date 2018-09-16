//
// Created by Yixiang Tu on 02/09/2018.
//

#include "ChipSizePopup.h"
#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QErrorMessage>

const QString placeholder[] = {"5-8", "5-8", "start from 0", "start from 0", "start from 0", "start from 0", "start from 0"};
const QString valid1[] = {"5", "6", "7", "8"};
const QString valid2[] = {"0", "1", "2", "3", "4", "5", "6", "7"};
const QString labels[] = {"rows", "cols", "in1", "in2", "out1", "out2", "out3"};

ChipSizePopup::ChipSizePopup() : QDialog() {

  setFixedSize(386, 256);

  auto layout = new QHBoxLayout(this);
  setLayout(layout);

  auto inputWidget = new QWidget(this);
  auto inputWidgetLayout = new QFormLayout(inputWidget);
  inputWidget->setLayout(inputWidgetLayout);
  {
    for (int i = 0; i < 7; ++i) {
      correct[i] = false;
      result[i] = -1;
      input[i] = new QLineEdit(inputWidget);
      input[i]->setPlaceholderText(placeholder[i]);
      inputWidgetLayout->addRow(new QLabel(labels[i], inputWidget), input[i]);
    }
  }
  layout->addWidget(inputWidget);

  auto button = new QPushButton(this);
  button->setText("OK");
  button->setAutoFillBackground(false);
  button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  layout->addWidget(button);

  connect(button, &QPushButton::clicked, [&]() {
    for (int i = 0; i < 2; ++i) {
      for (auto& v: valid1) {
        if (v == input[i]->text()) {
          result[i] = v.toInt();
          correct[i] = true;
          break;
        }
      }
    }
    for (int i = 2; i < 7; ++i) {
      for (auto& v: valid2) {
        if (v == input[i]->text()) {
          result[i] = v.toInt();
          correct[i] = result[i] < result[1];
          break;
        }
      }
    }
    if (result[2] == result[3]) {
      correct[2] = correct[3] = false;
    }
    if (result[4] == result[5]) {
      correct[4] = correct[5] = false;
    }
    if (result[5] == result[6]) {
      correct[5] = correct[6] = false;
    }
    if (result[4] == result[6]) {
      correct[4] = correct[6] = false;
    }
    bool bad = false;
    for (int i = 0; i < 7; ++i) {
      if (!correct[i]) {
        bad = true;
        input[i]->setText("");
      }
    }
    if (!bad) {
      completed = true;
      accept();
    }
  });

}
