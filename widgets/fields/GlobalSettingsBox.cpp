//
// Created by Yixiang Tu on 01/09/2418.
//

#include "GlobalSettingsBox.h"
#include "ControlPanel.h"
#include "../../Chip.h"
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>

GlobalSettingsBox::GlobalSettingsBox(QWidget* parent) : QWidget(parent) {
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  auto leftBox = new QGroupBox(this);
  auto leftBoxLayout = new QVBoxLayout(leftBox);
  {
    auto chipSizeWidget = new QWidget(leftBox);
    auto chipSizeWidgetLayout = new QHBoxLayout(chipSizeWidget);
    chipSizeWidgetLayout->setAlignment(Qt::AlignVCenter);
    chipSizeWidgetLayout->setContentsMargins(0, 0, 0, 0);
    {
      auto chipSizeInputWidget = new QWidget(chipSizeWidget);
      auto chipSizeInputWidgetLayout = new QFormLayout(chipSizeInputWidget);
      chipSizeInputWidgetLayout->setContentsMargins(0, 0, 0, 0);
      {
        rowInput = new QSpinBox(chipSizeInputWidget);
        rowInput->setValue(8);
        rowInput->setRange(5, 8);
        chipSizeInputWidgetLayout->addRow(new QLabel("rows", chipSizeInputWidget), rowInput);
        colInput = new QSpinBox(chipSizeInputWidget);
        colInput->setValue(8);
        colInput->setRange(5, 8);
        chipSizeInputWidgetLayout->addRow(new QLabel("cols", chipSizeInputWidget), colInput);
      }
      chipSizeInputWidget->setLayout(chipSizeInputWidgetLayout);
      chipSizeWidgetLayout->addWidget(chipSizeInputWidget);

      chipSizeButton = new QPushButton(chipSizeWidget);
      {
        chipSizeButton->setText("set");
        chipSizeButton->setFont(QFont("Helvetica", 15));
        chipSizeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        connect(chipSizeButton, &QPushButton::clicked, [this]() {
          auto r = rowInput->value();
          auto c = colInput->value();
          if (5 <= r and r <= 8 and 5 <= c and c <= 8) {
            emit controlPanelEvent(ControlPanelEvent(r, c));
          } else {
            emit controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::badInput,
                                                     "Invalid chip size!"));
          }
        });
      }
      chipSizeWidgetLayout->addWidget(chipSizeButton);
    }
    chipSizeWidget->setLayout(chipSizeWidgetLayout);
    leftBoxLayout->addWidget(chipSizeWidget);

    {
      auto button = new QPushButton(leftBox);

      button->setText("randomize map");
      button->setFont(QFont("Helvetica", 15));
      button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
      connect(button, &QPushButton::clicked,
              [&]() { emit controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::randomizeMap)); });

      leftBoxLayout->addWidget(button);
    }

    {
      auto button = new QPushButton(leftBox);

      button->setText("randomize channel size");
      button->setFont(QFont("Helvetica", 15));
      button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
      connect(button, &QPushButton::clicked,
              [&]() { emit controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::randomizePipeSize)); });

      leftBoxLayout->addWidget(button);
    }

  }
  leftBox->setLayout(leftBoxLayout);

  auto rightBox = new QGroupBox(this);
  auto rightBoxLayout = new QGridLayout(rightBox);
  {
    {
      auto button = new QPushButton(rightBox);

      button->setText("clear redundant channels");
      button->setFont(QFont("Helvetica", 15));
      button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
      connect(button, &QPushButton::clicked,
              [&]() { emit controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::clearPipes)); });

      rightBoxLayout->addWidget(button);
    }

    {
      auto button = new QPushButton(rightBox);

      button->setText("calculate");
      button->setFont(QFont("Helvetica", 24));
      button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
      connect(button, &QPushButton::clicked,
              [&]() { emit controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::calculate)); });

      rightBoxLayout->addWidget(button);
    }

    {
      auto button = new QPushButton(rightBox);

      button->setText("reset");
      button->setFont(QFont("Helvetica", 24));
      button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
      connect(button, &QPushButton::clicked,
              [&]() { emit controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::reset)); });

      rightBoxLayout->addWidget(button);
    }

  }
  rightBox->setLayout(rightBoxLayout);

  layout->addWidget(leftBox);
  layout->addWidget(rightBox);
  setLayout(layout);
}

void GlobalSettingsBox::updateSettings() {
  rowInput->setValue(CHIP.rows);
  colInput->setValue(CHIP.cols);
  rowInput->repaint();
  colInput->repaint();
}
