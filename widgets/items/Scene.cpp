//
// Created by Yixiang Tu on 31/08/2018.
//

#include "Scene.h"
#include "Pipe.h"
#include "PipeHighlight.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* ev) {

  auto target = itemAt(ev->scenePos(), QTransform());
  if (target and target->type() == ItemType::pipeHighlight) {
    if (ev->button() == Qt::LeftButton) {
      emit pipeClick(dynamic_cast<PipeHighlight*>(target)->pipe);
    } else {
      emit pipeRightClick(dynamic_cast<PipeHighlight*>(target)->pipe);
    }
  } else {
    emit defaultClick();
  }
  //QGraphicsScene::mousePressEvent(ev);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent* ev) {
  auto target = itemAt(ev->scenePos(), QTransform());
  if (target and target->type() == ItemType::pipeHighlight) {
    emit pipeMouseMove(dynamic_cast<PipeHighlight*>(target)->pipe);
  } else {
    emit defaultMouseMove();
  }
}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* ev) {
  mousePressEvent(ev);
}

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* ev) {
  emit mouseRelease();
}
