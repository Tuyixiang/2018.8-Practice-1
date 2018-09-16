//
// Created by Yixiang Tu on 29/08/2018.
//

#include "Toolkit.h"
#include "../items/ToolkitButton.h"
#include <QDebug>
#include <QMouseEvent>

Toolkit::Toolkit(QWidget* parent) : QGraphicsView(parent) {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  scene = new QGraphicsScene(this);
  setScene(scene);
  QTransform transform;
  transform.scale(0.1, 0.1);
  setTransform(transform);
  selectButton = new ToolkitButton(this, scene, "select", ":/img/0.png");
  selectButton->setPos(0, -720);
  selectButton->setActive();
  addButton = new ToolkitButton(this, scene, "add", ":/img/1.png");
  removeButton = new ToolkitButton(this, scene, "remove", ":/img/2.png");
  removeButton->setPos(0, 720);
  connect(selectButton, &ToolkitButton::mousePress,
          [&]() {
            selectButton->setActive();
            addButton->setInactive();
            removeButton->setInactive();
            emit toolSwitch(Tools::select);
          });
  connect(addButton, &ToolkitButton::mousePress,
          [&]() {
            selectButton->setInactive();
            addButton->setActive();
            removeButton->setInactive();
            emit toolSwitch(Tools::add);
          });
  connect(removeButton, &ToolkitButton::mousePress,
          [&]() {
            selectButton->setInactive();
            addButton->setInactive();
            removeButton->setActive();
            emit toolSwitch(Tools::remove);
          });
}

void Toolkit::mouseMoveEvent(QMouseEvent* ev) {
  if (itemAt(ev->pos())) {
    setCursor(Qt::PointingHandCursor);
  } else {
    setCursor(Qt::ArrowCursor);
  }
}
