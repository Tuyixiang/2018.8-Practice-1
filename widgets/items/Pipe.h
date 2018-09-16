//
// Created by Yixiang Tu on 29/08/2018.
//

#ifndef CHIP_SIMULATOR_PIPE_H
#define CHIP_SIMULATOR_PIPE_H

#include <QGraphicsItem>
#include <QDebug>
#include <QBrush>
#include "../../Chip.h"
#include "../../EnumTypes.h"

class QPropertyAnimation;

class Chip;

class PipeHighlight;

struct PipeInfo;

class Pipe : public QObject, public QGraphicsItemGroup {

Q_OBJECT
  Q_PROPERTY(QPointF pos
               READ
                 pos
               WRITE
               setPos)
  Q_PROPERTY(QSize size WRITE setSize)
  Q_PROPERTY(QColor color
               WRITE
               setColor)

  friend class Display;

  friend class LogicHandler;

protected:
  static const QColor colorDisabled;
  static const QColor colorEnabled;
  static const QColor colorDefunct;
  static const QColor colorInvisible;

public:

  Pipe(QObject* parent, QGraphicsScene* s, PipeInfo* i);

  void initializePolygon(PipeType t, int x, int y);

  void initializePosition();

  void initializeHighlight();

  void setColor(const QColor& c) {
    _color = c;
    polygon->setBrush(getBrush());
  }

  void setSize(const QSize& s);

  void setAsVisible() {
    if (!visible) {
      visible = true;
      polygon->show();
      emit visibilityChanged();
    }
  }

  void setAsInvisible() {
    if (visible) {
      visible = false;
      polygon->hide();
      emit visibilityChanged();
    }
  }

  void turnOnHighlight();

  void turnOffHighlight();

  int type() const override {
    return ItemType::pipe;
  }

  QPointF realPos() const;

  ~Pipe();

signals:

  /// emitted when <b>PipeInfo</b> is changed
  void infoChanged();

  /// emitted when display properties are changed<br>
  /// (for example, change of color)
  void propChanged();

  /// emitted when causing changes in the overall chip<br>
  /// (for example, change of length of pipes)
  void chipChanged();

  void visibilityChanged();

  void mousePress(Pipe*);

public slots:

  void updateToolTip();

  void disabilitySwitch();

  void startAnimation();

  void calculateTempPosition();

public:
  PipeInfo* const info;
  bool visible = true;
  int xIndex = 0;
  int yIndex = 0;
  PipeType pipeType = PipeType::null;

protected:
  QGraphicsScene* const scene;

  /// display parts
  QGraphicsPolygonItem* polygon = nullptr;
  PipeHighlight* highlight;

  /// in order to display animations, below are the current properties on display
  QColor _color;
  int _width;
  int _length;

  /// here are the destination of animations
  QColor _destColor;

  /// animation handlers
  QPropertyAnimation* colorAnimation = nullptr;
  QPropertyAnimation* sizeAnimation = nullptr;
  QPropertyAnimation* posAnimation = nullptr;

  QBrush getBrush();

  QVector<QPointF> getShape();

  void checkDestColor();

  void mousePressEvent(QGraphicsSceneMouseEvent* ev) override;

};

#endif //CHIP_SIMULATOR_PIPE_H
