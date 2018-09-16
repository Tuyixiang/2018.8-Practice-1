//
// Created by Yixiang Tu on 28/08/2018.
//

#include "ControlPanel.h"
#include "FocusView.h"
#include "FocusViewDescriptionBox.h"
#include "GlobalSettingsBox.h"
#include <QPushButton>
#include <QLayout>
#include <QDebug>
#include <QGroupBox>
#include <QLabel>
#include <QRubberBand>

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent) {

  auto layout = new QVBoxLayout(this);
  layout->setAlignment(Qt::AlignHCenter);
  layout->setContentsMargins(0, 0, 0, 0);

  auto focusBox = new QGroupBox(this);
  auto focusBoxLayout = new QHBoxLayout(focusBox);
  focusBoxLayout->setAlignment(Qt::AlignVCenter);
  focusBox->setLayout(focusBoxLayout);

  focusView = new FocusView(focusBox);
  focusViewDescriptionBox = new FocusViewDescriptionBox(focusBox);
  connect(focusViewDescriptionBox, &FocusViewDescriptionBox::controlPanelEvent,
          [&](const ControlPanelEvent& cpe) { emit controlPanelEvent(cpe); });

  focusBoxLayout->addWidget(focusView);
  focusBoxLayout->addWidget(focusViewDescriptionBox);

  globalSettingsBox = new GlobalSettingsBox(this);
  connect(globalSettingsBox, &GlobalSettingsBox::controlPanelEvent,
          [&](const ControlPanelEvent& cpe) { emit controlPanelEvent(cpe); });

  layout->addWidget(focusBox);
  layout->addWidget(globalSettingsBox);


  setLayout(layout);
}

void ControlPanel::setScene(QGraphicsScene* s) {
  focusView->setScene(s);
  focusView->initializeFocus();
}
