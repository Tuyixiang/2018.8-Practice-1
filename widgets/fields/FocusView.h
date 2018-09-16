//
// Created by Yixiang Tu on 30/08/2018.
//

#ifndef CHIP_SIMULATOR_FOCUSVIEW_H
#define CHIP_SIMULATOR_FOCUSVIEW_H

#include <QGraphicsView>
#include "../items/Pipe.h"

class QPropertyAnimation;

class PipeHintBoard : public QObject{

  Q_OBJECT
  Q_PROPERTY(qreal opacity WRITE setOpacity)

  friend class FocusView;

public:
  PipeHintBoard(QObject* parent, Pipe* t) : QObject(parent), target(t) {
    start();
  }

  void paint(QPainter* painter);

  void setOpacity(qreal o) {
    opacity = o;
  }

  /// only call once
  void start();

  void stop();

  /// once finished, this object should be removed from list
  bool finished = false;

protected:
  QPropertyAnimation* animation = nullptr;
  Pipe* target = nullptr;
  qreal opacity = 0;
};

class FocusView : public QGraphicsView {

Q_OBJECT
  Q_PROPERTY(QPointF focusPoint
               WRITE
               setFocusPoint)
  Q_PROPERTY(qreal focusScale
               WRITE
               setFocusScale)
  Q_PROPERTY(qreal defaultMessageOpacity WRITE setDefaultMessageOpacity)

public:
  FocusView(QWidget* parent);

  QSize sizeHint() const override {
    return QSize(256, 256);
  }

  void setFocusPoint(const QPointF& fp) {
    _focusPoint = fp;
    centerOn(fp);
  }

  void setFocusScale(qreal fs) {
    _focusScale = fs;
    QTransform transform;
    transform.scale(fs, fs);
    setTransform(transform);
  }

  void setDefaultMessageOpacity(qreal dmo) {
    _defaultMessageOpacity = dmo;
  }

  void startZoomAnimation(const QPointF& fp, qreal fs);

  void resetFocus();

  void initializeFocus();

public slots:
  void focusOnPipe(Pipe* target);

  void setTool(Tools t);

protected:

  void showDefaultMessage();

  void hideDefaultMessage();

  void drawForeground(QPainter* painter, const QRectF& rect) override;

  void wheelEvent(QWheelEvent* ev) override {
      (void)ev;
  }
  
  Pipe* focusTarget = nullptr;

  QGraphicsScene* scene = nullptr;
  QPointF _focusPoint;
  qreal _focusScale;
  qreal _defaultMessageOpacity = 1;
  QVector<PipeHintBoard*> pipeHintBoards;
  QPropertyAnimation* focusPointAnimation = nullptr;
  QPropertyAnimation* focusScaleAnimation = nullptr;
  QPropertyAnimation* defaultMessageAnimation = nullptr;

};



#endif //CHIP_SIMULATOR_FOCUSVIEW_H
