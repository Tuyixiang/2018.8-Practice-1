//
// Created by Yixiang Tu on 30/08/2018.
//

#include "FocusView.h"
#include "../../Chip.h"
#include "../items/Pipe.h"
#include "../../MyMath.h"
#include <QPropertyAnimation>
#include <QPaintEvent>

FocusView::FocusView(QWidget* parent) : QGraphicsView(parent) {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setMouseTracking(true);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void FocusView::initializeFocus() {
  setSceneRect(-8000, -8000, 22800, 22800);
  setFocusScale(0.016);
  setFocusPoint(QPointF(CHIP.accHLength[CHIP.cols - 1] / 2.0, CHIP.accVLength[CHIP.rows - 1] / 2.0));
  resetFocus();
}

void FocusView::resetFocus() {
  focusTarget = nullptr;
  startZoomAnimation(QPointF(CHIP.accHLength[CHIP.cols - 1] / 2.0, CHIP.accVLength[CHIP.rows - 1] / 2.0), 0.016);
}

void FocusView::startZoomAnimation(const QPointF& fp, qreal fs) {
  auto easingStrategy = QEasingCurve::InOutQuad;
  if (focusPointAnimation and focusPointAnimation->state() == focusPointAnimation->Running) {
    easingStrategy = QEasingCurve::OutQuad;
  }
  delete focusPointAnimation;
  focusPointAnimation = nullptr;
  delete focusScaleAnimation;
  focusScaleAnimation = nullptr;
  if (fp != _focusPoint) {
    focusPointAnimation = new QPropertyAnimation(this, "focusPoint");
    focusPointAnimation->setDuration(600);
    focusPointAnimation->setStartValue(_focusPoint);
    focusPointAnimation->setEndValue(fp);
    focusPointAnimation->setEasingCurve(easingStrategy);
    focusPointAnimation->start();
  }
  if (fs != _focusScale) {
    focusScaleAnimation = new QPropertyAnimation(this, "focusScale");
    focusScaleAnimation->setDuration(600);
    focusScaleAnimation->setStartValue(_focusScale);
    focusScaleAnimation->setEndValue(fs);
    focusScaleAnimation->setEasingCurve(easingStrategy);
    focusScaleAnimation->start();
  }
}

void FocusView::focusOnPipe(Pipe* target) {
  if (!target or focusTarget != target or target->pos() != target->realPos()) {
    for (auto it = pipeHintBoards.begin(); it != pipeHintBoards.cend();) {
      if ((*it)->finished) {
        delete *it;
        it = pipeHintBoards.erase(it);
      } else {
        (*it)->stop();
        ++it;
      }
    }
    focusTarget = target;
    if (target) {
      startZoomAnimation(target->realPos(), 160.0 / target->info->length);
      if (target->info->defunct or (!target->info->disabled and CHIP.calculated and !target->info->rate)) {
        pipeHintBoards.push_back(new PipeHintBoard(this, target));
      }
      hideDefaultMessage();
    } else {
      showDefaultMessage();
      resetFocus();
    }
  }
}

void FocusView::drawForeground(QPainter* painter, const QRectF& rect) {
  (void)rect;
  painter->setPen(Qt::NoPen);
  if (_defaultMessageOpacity) {
    painter->setBrush(QBrush(QColor(255, 255, 255, static_cast<int>(_defaultMessageOpacity * 200))));
    auto rec = MyMath::rectFromCenter(CHIP.tempAccHLength[CHIP.cols - 1] / 2.0, CHIP.tempAccVLength[CHIP.rows - 1] / 2.0, 10000, 4000);
    painter->drawRect(rec);
    painter->setPen(QColor(0, 0, 0, static_cast<int>(_defaultMessageOpacity * 255)));
    painter->setFont(QFont("Helvetica", 1000, 10));
    painter->drawText(rec, Qt::AlignCenter, "nothing selected");
  }
  for (auto it = pipeHintBoards.begin(); it != pipeHintBoards.cend();) {
    if ((*it)->finished) {
      delete *it;
      it = pipeHintBoards.erase(it);
    } else {
      (*it)->paint(painter);
      ++it;
    }
  }
}

void FocusView::showDefaultMessage() {
  if (_defaultMessageOpacity != 1) {
    delete defaultMessageAnimation;
    defaultMessageAnimation = new QPropertyAnimation(this, "defaultMessageOpacity");
    defaultMessageAnimation->setStartValue(_defaultMessageOpacity);
    defaultMessageAnimation->setEndValue(1);
    defaultMessageAnimation->setDuration(500);
    defaultMessageAnimation->setEasingCurve(QEasingCurve::OutQuad);
    defaultMessageAnimation->start();
  }
}

void FocusView::hideDefaultMessage() {
  if (_defaultMessageOpacity) {
    delete defaultMessageAnimation;
    defaultMessageAnimation = new QPropertyAnimation(this, "defaultMessageOpacity");
    defaultMessageAnimation->setStartValue(_defaultMessageOpacity);
    defaultMessageAnimation->setEndValue(0);
    defaultMessageAnimation->setDuration(500);
    defaultMessageAnimation->setEasingCurve(QEasingCurve::OutQuad);
    defaultMessageAnimation->start();
  }
}

void FocusView::setTool(Tools t) {
  focusOnPipe(nullptr);
  switch (t) {
    case Tools::select: {
      break;
    }
    case Tools::add: {
      startZoomAnimation(QPointF(CHIP.accHLength[CHIP.cols - 1] / 2.0, CHIP.accVLength[CHIP.rows - 1] / 2.0), 0.012);
      hideDefaultMessage();
      break;
    }
    case Tools::remove: {
      hideDefaultMessage();
      startZoomAnimation(QPointF(CHIP.accHLength[CHIP.cols - 1] / 2.0, CHIP.accVLength[CHIP.rows - 1] / 2.0), 0.012);
      break;
    }
  }
}

void PipeHintBoard::paint(QPainter* painter) {
  auto rect = MyMath::rectFromCenter(target->realPos(), 1600, 1200);
  painter->setBrush(QBrush(QColor(255, 255, 180, static_cast<int>(180 * opacity))));
  painter->setPen(QColor(0, 0, 0, static_cast<int>(230 * opacity)));
  painter->drawRect(rect);
  painter->setFont(QFont("Helvetica", 140, 10));
  painter->drawText(rect, Qt::AlignCenter, "This channel may be\nredundant since there\nwill not be any current.");
}

void PipeHintBoard::start() {
  animation = new QPropertyAnimation(this, "opacity");
  animation->setDuration(500);
  animation->setStartValue(0);
  animation->setEndValue(1);
  animation->start();
}

void PipeHintBoard::stop() {
  if (animation->endValue() != 0) {
    delete animation;
    animation = new QPropertyAnimation(this, "opacity");
    animation->setDuration(500);
    animation->setStartValue(opacity);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, [this]() mutable { finished = true; });
  }
}
