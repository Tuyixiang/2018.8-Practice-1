//
// Created by Yixiang Tu on 31/08/2018.
//

#ifndef CHIP_SIMULATOR_SCENE_H
#define CHIP_SIMULATOR_SCENE_H
#include <QGraphicsScene>

class Pipe;

class Scene : public QGraphicsScene {

  Q_OBJECT

signals:
  void pipeClick(Pipe*);
  void pipeRightClick(Pipe*);
  void defaultClick();
  void pipeMouseMove(Pipe*);
  void defaultMouseMove();
  void mouseRelease();

public:
  Scene(QObject* parent);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent* ev) override;

  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* ev) override;

  void mouseMoveEvent(QGraphicsSceneMouseEvent* ev) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent* ev) override;

};


#endif //CHIP_SIMULATOR_SCENE_H
