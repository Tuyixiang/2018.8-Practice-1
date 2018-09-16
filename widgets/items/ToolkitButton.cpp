//
// Created by Yixiang Tu on 30/08/2018.
//

#include "ToolkitButton.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QPropertyAnimation>

ToolkitButton::ToolkitButton(QObject* parent, QGraphicsScene* s, const QString& label, const QString& iconFilename) :
  QObject(parent),
  QGraphicsItemGroup(nullptr),
  scene(s) {
  scene->addItem(this);
  shape = scene->addRect(-320, -320, 640, 640, QPen(QColor(0, 0, 0, 128)), QBrush(Qt::white));
  icon = scene->addRect(0, 0, 640, 640, Qt::NoPen, QBrush(QImage(iconFilename)));
  icon->setPos(-320, -320);
  text = scene->addText(label, QFont("Helvetica", 128));
  text->setPos(-text->boundingRect().width() / 2.0, 180 - text->boundingRect().height() / 2.0);
  addToGroup(shape);
  addToGroup(icon);
  addToGroup(text);

}

void ToolkitButton::setActive() {
  if (!active) {
    delete animation;
    animation = new QPropertyAnimation(this, "backgroundOpacity");
    animation->setDuration(250);
    animation->setStartValue(_backgroundOpacity);
    animation->setEndValue(1);
    animation->start();
    active = true;
  }
}

void ToolkitButton::setBackgroundOpacity(qreal o) {
  _backgroundOpacity = o;
  shape->setBrush(QBrush(QColor(212, 246, 255, static_cast<int>(255 * o))));
}

void ToolkitButton::setInactive() {
  if (active) {
    delete animation;
    animation = new QPropertyAnimation(this, "backgroundOpacity");
    animation->setDuration(250);
    animation->setStartValue(_backgroundOpacity);
    animation->setEndValue(0);
    animation->start();
    active = false;
  }
}
