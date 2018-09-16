//
// Created by Yixiang Tu on 28/08/2018.
//

#ifndef CHIP_SIMULATOR_DISPLAY_H
#define CHIP_SIMULATOR_DISPLAY_H

#include <QGraphicsView>

class QGraphicsScene;

class QGraphicsView;

class Pipe;

class Scene;

class Display : public QGraphicsView {

Q_OBJECT

  friend class MainWindow;

  friend class LogicHandler;

public:
  explicit Display(QWidget* parent);

  QSize sizeHint() const override {
    return QSize(512, 576);
  }

  ~Display() override;

  void initializePipes();

  QVector<Pipe*> pipes();

public slots:

  void startAnimation();

  void updatePipePosition();

  void turnOnHighlight(Pipe* target);

  void turnOffHighlight(Pipe* target);

  void switchDisability(Pipe* target);

  void resetFocus();

protected:
  void paintEvent(QPaintEvent* ev) override;

  Scene* scene = nullptr;

  Pipe* inPipe[8];
  Pipe* outPipe[8];
  /// the indices indicate the top-left location of the pipe
  Pipe* vPipe[8][7];
  /// the indices indicate the top-left location of the pipe
  Pipe* hPipe[7][8];

};


#endif //CHIP_SIMULATOR_DISPLAY_H
