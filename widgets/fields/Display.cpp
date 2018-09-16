//
// Created by Yixiang Tu on 28/08/2018.
//

#include "Display.h"
#include "../items/Pipe.h"
#include "../items/Scene.h"
#include "../../Chip.h"
#include "../../EnumTypes.h"
#include "../../MyMath.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QLayout>
#include <QPropertyAnimation>
#include <QTimer>

Display::Display(QWidget* parent) : QGraphicsView(parent) {
  scene = new Scene(this);
  //scene->addRect(0, 0, 100, 100, QPen(QColor(0, 0, 0)), QBrush(QColor(50, 50, 50)));
  //scene->addRect(0, 150, 1000, 1000, QPen(QColor(0, 0, 0)), QBrush(QColor(50, 50, 50)))->setToolTip("123");
  setScene(scene);
  //setSceneRect(MyMath::rectFromCenter(CHIP.accHLength[CHIP.cols - 1] / 2.0, CHIP.accVLength[CHIP.rows - 1] / 2.0, 20000, 20000));
  QTransform transform;
  transform.scale(0.032, 0.032);
  setTransform(transform);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  //qDebug() << "view: " << view->width() << view->height();

  initializePipes();

  setMouseTracking(true);

}

Display::~Display() {

}

void Display::initializePipes() {
  for (int i = 0; i < 8; ++i) {
    inPipe[i] = new Pipe(this, scene, CHIP.inPipe + i);
    outPipe[i] = new Pipe(this, scene, CHIP.outPipe + i);
    for (int j = 0; j < 7; ++j) {
      vPipe[i][j] = new Pipe(this, scene, CHIP.vPipe[i] + j);
      hPipe[j][i] = new Pipe(this, scene, CHIP.hPipe[j] + i);
    }
  }
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 7; ++j) {
      vPipe[i][j]->initializePolygon(PipeType::vertical, i, j);
      hPipe[j][i]->initializePolygon(PipeType::horizontal, j, i);
    }
    inPipe[i]->initializePolygon(PipeType::input, i, 0);
    outPipe[i]->initializePolygon(PipeType::output, i, 0);
  }

  for (auto* pipe : pipes()) {
    pipe->initializeHighlight();
  }

  for (auto* pipe : pipes()) {
    pipe->initializePosition();
  }
}

void Display::startAnimation() {
  for (auto* p: pipes()) {
    p->startAnimation();
    p->updateToolTip();
  }
}

QVector<Pipe*> Display::pipes() {
  static QVector<Pipe*> p;
  if (p.isEmpty()) {
    for (int i = 0; i < 8; ++i) {
      p.push_back(inPipe[i]);
      p.push_back(outPipe[i]);
      for (int j = 0; j < 7; ++j) {
        p.push_back(vPipe[i][j]);
        p.push_back(hPipe[j][i]);
      }
    }
  }
  return p;
}

void Display::turnOnHighlight(Pipe* target) {
  target->turnOnHighlight();
}

void Display::turnOffHighlight(Pipe* target) {
  target->turnOffHighlight();
}

void Display::switchDisability(Pipe* target) {
  target->disabilitySwitch();
}

void Display::updatePipePosition() {
  CHIP.calcTempAccLengths();
  for (auto* p: pipes()) {
    p->calculateTempPosition();
  }
}

void Display::paintEvent(QPaintEvent* ev) {
  QGraphicsView::paintEvent(ev);
  if (CHIP.sizeChanging) {
    updatePipePosition();
  }
}

void Display::resetFocus() {
  //QTimer::singleShot(600, [&]() {
  //  centerOn(CHIP.accHLength[CHIP.cols - 1] / 2.0, CHIP.accVLength[CHIP.rows - 1] / 2.0);
  //});
}
