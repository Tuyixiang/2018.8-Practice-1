//
// Created by Yixiang Tu on 30/08/2018.
//

#ifndef CHIP_SIMULATOR_TOOLKITBUTTON_H
#define CHIP_SIMULATOR_TOOLKITBUTTON_H

#include <QGraphicsItem>
#include <QObject>

class QPropertyAnimation;

class ToolkitButton : public QObject, public QGraphicsItemGroup {

Q_OBJECT

  Q_PROPERTY(qreal backgroundOpacity
               WRITE
               setBackgroundOpacity)

signals:

  void mousePress();

public:
  ToolkitButton(QObject* parent, QGraphicsScene* s, const QString& label, const QString& iconFilename);

  void setActive();

  void setInactive();

protected:

  void setBackgroundOpacity(qreal o);

  void mousePressEvent(QGraphicsSceneMouseEvent* ev) override {
    emit mousePress();
  }

  bool active = false;

  qreal _backgroundOpacity = 0;

  QGraphicsScene* scene = nullptr;
  QGraphicsRectItem* icon = nullptr;
  QGraphicsRectItem* shape = nullptr;
  QGraphicsTextItem* text = nullptr;
  QPropertyAnimation* animation = nullptr;

};


#endif //CHIP_SIMULATOR_TOOLKITBUTTON_H
