//
// Created by Yixiang Tu on 31/08/2018.
//

#ifndef CHIP_SIMULATOR_PIPEHIGHLIGHT_H
#define CHIP_SIMULATOR_PIPEHIGHLIGHT_H
#include <QGraphicsItemGroup>
#include <QObject>
#include "Pipe.h"

struct PipeInfo;
class QPropertyAnimation;

class PipeHighlight : public QObject, public QGraphicsItemGroup {

  Q_OBJECT
  Q_PROPERTY(qreal highlightOpacity
               READ
                 highlightOpacity
               WRITE
               setHighlightOpacity)
public:
  PipeHighlight(QObject* parent, QGraphicsScene* scene, Pipe* p);

  void initialize();

  qreal highlightOpacity() const {
    return _highlightOpacity;
  }

  void setHighlightOpacity(qreal ho);

  void turnOn();

  void turnOff();

  void setSize(int width, int length);

  int type() const override {
    return ItemType::pipeHighlight;
  }

public:
  Pipe* pipe;

protected:

  QBrush getHighlightBrush(int index);

  QVector<QPointF> getHightlightShape(int index);

  QPropertyAnimation* opacityLoopAnimation = nullptr;
  QPropertyAnimation* opacityOnOffAnimation = nullptr;
  int _width;
  int _length;
  PipeType pipeType;
  PipeInfo* info;
  QGraphicsScene* scene;
  QGraphicsPolygonItem* highlight[13] = {nullptr};
  qreal _highlightOpacity = 1;


};


#endif //CHIP_SIMULATOR_PIPEHIGHLIGHT_H
