//
// Created by Yixiang Tu on 31/08/2018.
//

#include "PipeHighlight.h"
#include "../../Chip.h"
#include "../../MyMath.h"
#include <QGraphicsScene>
#include <QPropertyAnimation>

void PipeHighlight::initialize() {
  _width = info->width;
  _length = info->length;
  if (pipeType == PipeType::input or pipeType == PipeType::output) {
    for (int i = 0; i < 13; ++i) {
      highlight[i] = scene->addPolygon(QPolygonF(getHightlightShape(i)), Qt::NoPen, getHighlightBrush(i));
      highlight[i]->setZValue(1);
      addToGroup(highlight[i]);
    }
  } else {
    for (int i = 0; i < 6; ++i) {
      highlight[i] = scene->addPolygon(QPolygonF(getHightlightShape(i)), Qt::NoPen, getHighlightBrush(i));
      highlight[i]->setZValue(1);
      addToGroup(highlight[i]);
    }
  }
  opacityLoopAnimation = new QPropertyAnimation(this, "highlightOpacity");
  opacityLoopAnimation->setStartValue(1);
  opacityLoopAnimation->setKeyValueAt(0.5, 0.5);
  opacityLoopAnimation->setEndValue(1);
  opacityLoopAnimation->setLoopCount(-1);
  opacityLoopAnimation->setDuration(1400);
  setHighlightOpacity(0);
  //opacityAnimation->start();
}

PipeHighlight::PipeHighlight(QObject* parent, QGraphicsScene* s, Pipe* p) :
  QObject(parent),
  QGraphicsItemGroup(nullptr),
  info(p->info),
  pipeType(p->pipeType),
  pipe(p),
  scene(s) {
  scene->addItem(this);
  initialize();
}

QBrush PipeHighlight::getHighlightBrush(int index) {
  QLinearGradient gradient;
  switch (pipeType) {
    case PipeType::input: {
      switch (index) {
        case 0: {
          gradient.setStart(0, -_length / 2.0);
          gradient.setFinalStop(0, -150 - _length / 2.0);
          break;
        }
        case 1: {
          gradient.setStart(_width / 2.0, 0);
          gradient.setFinalStop(150 + _width / 2.0, 0);
          break;
        }
        case 2: {
          gradient.setStart(_width / 2.0, -_width / MyMath::sqrt(2.0));
          gradient.setFinalStop(-150 / MyMath::sqrt(2.0) + _width / 2.0,
                                -150 / MyMath::sqrt(2.0) - _width / MyMath::sqrt(2.0));
          break;
        }
        case 3: {
          gradient.setStart(_width * 3.0 / 2.0, 0);
          gradient.setFinalStop(150 + _width * 3.0 / 2.0, 0);
          break;
        }
        case 4: {
          gradient.setStart(_width / 2.0, _width / MyMath::sqrt(2.0));
          gradient.setFinalStop(150 / MyMath::sqrt(2.0) + _width / 2.0,
                                150 / MyMath::sqrt(2.0) + _width / MyMath::sqrt(2.0));
          break;
        }
        case 5: {
          gradient.setStart(_width / 2.0, 0);
          gradient.setFinalStop(150 + _width / 2.0, 0);
          break;
        }
        case 6: {
          gradient.setStart(_width / 2.0, _length / 2.0 - _width / 2.0);
          gradient.setFinalStop(150 / MyMath::sqrt(2.0) + _width / 2.0,
                                150 / MyMath::sqrt(2.0) + _length / 2.0 - _width / 2.0);
          break;
        }
        case 7: {
          gradient.setStart(-(_width / 2.0), _length / 2.0 - _width / 2.0);
          gradient.setFinalStop(-(150 / MyMath::sqrt(2.0) + _width / 2.0),
                                150 / MyMath::sqrt(2.0) + _length / 2.0 - _width / 2.0);
          break;
        }
        case 8: {
          gradient.setStart(-(_width / 2.0), 0);
          gradient.setFinalStop(-(150 + _width / 2.0), 0);
          break;
        }
        case 9: {
          gradient.setStart(-(_width / 2.0), _width / MyMath::sqrt(2.0));
          gradient.setFinalStop(-(150 / MyMath::sqrt(2.0) + _width / 2.0),
                                150 / MyMath::sqrt(2.0) + _width / MyMath::sqrt(2.0));
          break;
        }
        case 10: {
          gradient.setStart(-(_width * 3.0 / 2.0), 0);
          gradient.setFinalStop(-(150 + _width * 3.0 / 2.0), 0);
          break;
        }
        case 11: {
          gradient.setStart(-(_width / 2.0), -_width / MyMath::sqrt(2.0));
          gradient.setFinalStop(-(-150 / MyMath::sqrt(2.0) + _width / 2.0),
                                -150 / MyMath::sqrt(2.0) - _width / MyMath::sqrt(2.0));
          break;
        }
        case 12: {
          gradient.setStart(-(_width / 2.0), 0);
          gradient.setFinalStop(-(150 + _width / 2.0), 0);
          break;
        }
        default: {
          throw std::range_error("pipe highlight index out of range");
        }
      }
      break;
    }
    case PipeType::output: {
      switch (index) {
        case 0: {
          gradient.setStart(0, _length / 2.0);
          gradient.setFinalStop(0, 150 + _length / 2.0);
          break;
        }
        case 1: {
          gradient.setStart(_width / 2.0, 0);
          gradient.setFinalStop(150 + _width / 2.0, 0);
          break;
        }
        case 2: {
          gradient.setStart(_width / 2.0, 0);
          gradient.setFinalStop(-150 / MyMath::sqrt(2.0) + _width / 2.0,
                                -150 / MyMath::sqrt(2.0));
          break;
        }
        case 3: {
          gradient.setStart(_width * 3.0 / 2.0, 0);
          gradient.setFinalStop(150 + _width * 3.0 / 2.0, 0);
          break;
        }
        case 4: {
          gradient.setStart(_width / 2.0, _width * MyMath::sqrt(2.0));
          gradient.setFinalStop(150 / MyMath::sqrt(2.0) + _width / 2.0,
                                150 / MyMath::sqrt(2.0) + _width * MyMath::sqrt(2.0));
          break;
        }
        case 5: {
          gradient.setStart(_width / 2.0, 0);
          gradient.setFinalStop(150 + _width / 2.0, 0);
          break;
        }
        case 6: {
          gradient.setStart(_width / 2.0, -_length / 2.0 + _width / 2.0);
          gradient.setFinalStop(150 / MyMath::sqrt(2.0) + _width / 2.0,
                                -(150 / MyMath::sqrt(2.0) + _length / 2.0 - _width / 2.0));
          break;
        }
        case 7: {
          gradient.setStart(-(_width / 2.0), -_length / 2.0 + _width / 2.0);
          gradient.setFinalStop(-(150 / MyMath::sqrt(2.0) + _width / 2.0),
                                -(150 / MyMath::sqrt(2.0) + _length / 2.0 - _width / 2.0));
          break;
        }
        case 8: {
          gradient.setStart(-(_width / 2.0), 0);
          gradient.setFinalStop(-(150 + _width / 2.0), 0);
          break;
        }
        case 9: {
          gradient.setStart(-(_width / 2.0), _width * MyMath::sqrt(2.0));
          gradient.setFinalStop(-(150 / MyMath::sqrt(2.0) + _width / 2.0),
                                150 / MyMath::sqrt(2.0) + _width * MyMath::sqrt(2.0));
          break;
        }
        case 10: {
          gradient.setStart(-(_width * 3.0 / 2.0), 0);
          gradient.setFinalStop(-(150 + _width * 3.0 / 2.0), 0);
          break;
        }
        case 11: {
          gradient.setStart(-(_width / 2.0), 0);
          gradient.setFinalStop(-(-150 / MyMath::sqrt(2.0) + _width / 2.0),
                                -150 / MyMath::sqrt(2.0));
          break;
        }
        case 12: {
          gradient.setStart(-(_width / 2.0), 0);
          gradient.setFinalStop(-(150 + _width / 2.0), 0);
          break;
        }
        default: {
          throw std::range_error("pipe highlight index out of range");
        }
      }
      break;
    }
    case PipeType::vertical: {
      switch (index) {
        case 0: {
          gradient.setStart(0, -_length / 2.0);
          gradient.setFinalStop(150 / MyMath::sqrt(2.0), -_length / 2.0 - 150 / MyMath::sqrt(2.0));
          break;
        }
        case 1: {
          gradient.setStart(_width / 2.0, 0);
          gradient.setFinalStop(_width / 2.0 + 150, 0);
          break;
        }
        case 2: {
          gradient.setStart(0, _length / 2.0);
          gradient.setFinalStop(150 / MyMath::sqrt(2.0), _length / 2.0 + 150 / MyMath::sqrt(2.0));
          break;
        }
        case 3: {
          gradient.setStart(0, _length / 2.0);
          gradient.setFinalStop(-150 / MyMath::sqrt(2.0), _length / 2.0 + 150 / MyMath::sqrt(2.0));
          break;
        }
        case 4: {
          gradient.setStart(-_width / 2.0, 0);
          gradient.setFinalStop(-_width / 2.0 - 150, 0);
          break;
        }
        case 5: {
          gradient.setStart(0, -_length / 2.0);
          gradient.setFinalStop(-150 / MyMath::sqrt(2.0), -_length / 2.0 - 150 / MyMath::sqrt(2.0));
          break;
        }
        default: {
          throw std::range_error("pipe highlight index out of range");
        }
      }
      break;
    }
    case PipeType::horizontal: {
      switch (index) {
        case 0: {
          gradient.setStart(-_length / 2.0, 0);
          gradient.setFinalStop(-_length / 2.0 - 150 / MyMath::sqrt(2.0), 150 / MyMath::sqrt(2.0));
          break;
        }
        case 1: {
          gradient.setStart(0, _width / 2.0);
          gradient.setFinalStop(0, _width / 2.0 + 150);
          break;
        }
        case 2: {
          gradient.setStart(_length / 2.0, 0);
          gradient.setFinalStop(_length / 2.0 + 150 / MyMath::sqrt(2.0), 150 / MyMath::sqrt(2.0));
          break;
        }
        case 3: {
          gradient.setStart(_length / 2.0, 0);
          gradient.setFinalStop(_length / 2.0 + 150 / MyMath::sqrt(2.0), -150 / MyMath::sqrt(2.0));
          break;
        }
        case 4: {
          gradient.setStart(0, -_width / 2.0);
          gradient.setFinalStop(0, -_width / 2.0 - 150);
          break;
        }
        case 5: {
          gradient.setStart(-_length / 2.0, 0);
          gradient.setFinalStop(-_length / 2.0 - 150 / MyMath::sqrt(2.0), -150 / MyMath::sqrt(2.0));
          break;
        }
        default: {
          throw std::range_error("pipe highlight index out of range");
        }
      }
      break;
    }
    default: {
      throw std::range_error("pipe highlight index out of range");
    }
  }
  gradient.setColorAt(0, QColor(0, 255, 0, static_cast<int>(255 * _highlightOpacity)));
  gradient.setColorAt(0.25, QColor(0, 255, 0, static_cast<int>(128 * _highlightOpacity)));
  gradient.setColorAt(0.5, QColor(0, 255, 0, static_cast<int>(64 * _highlightOpacity)));
  gradient.setColorAt(0.75, QColor(0, 255, 0, static_cast<int>(32 * _highlightOpacity)));
  gradient.setColorAt(1, QColor(0, 255, 0, 0));
  return QBrush(gradient);
}

QVector<QPointF> PipeHighlight::getHightlightShape(int index) {
  QVector<QPointF> shape;
  switch (pipeType) {
    case PipeType::input: {
      switch (index) {
        case 0: {
          shape.push_back(QPointF(150 + _width / 2.0, -150 - _length / 2.0));
          shape.push_back(QPointF(_width / 2.0, -_length / 2.0));
          shape.push_back(QPointF(-_width / 2.0, -_length / 2.0));
          shape.push_back(QPointF(-150 - _width / 2.0, -150 - _length / 2.0));
          break;
        }
        case 1: {
          shape.push_back(QPointF(150 + _width / 2.0, -150 - _length / 2.0));
          shape.push_back(
            QPointF(150 + _width / 2.0, -150 * MyMath::sqrt(2.0) - 150 - _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(_width / 2.0, -_width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(_width / 2.0, -_length / 2.0));
          break;
        }
        case 2: {
          shape.push_back(
            QPointF(150 + _width / 2.0, -150 * MyMath::sqrt(2.0) - 150 - _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(150 + _width * 3.0 / 2.0,
                                  -150 * MyMath::sqrt(2.0) - 150 - _width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(_width * 3.0 / 2.0,
                                  -_width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(_width / 2.0, -_width / MyMath::sqrt(2.0)));
          break;
        }
        case 3: {
          shape.push_back(QPointF(150 + _width * 3.0 / 2.0,
                                  -150 * MyMath::sqrt(2.0) - 150 - _width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(150 + _width * 3.0 / 2.0,
                                  150 * MyMath::sqrt(2.0) - 150 + _width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(_width * 3.0 / 2.0,
                                  _width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(_width * 3.0 / 2.0,
                                  -_width / MyMath::sqrt(2.0) - _width));
          break;
        }
        case 4: {
          shape.push_back(QPointF(150 + _width * 3.0 / 2.0,
                                  150 * MyMath::sqrt(2.0) - 150 + _width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  150 * MyMath::sqrt(2.0) - 150 + _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(_width / 2.0, _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(_width * 3.0 / 2.0, _width / MyMath::sqrt(2.0) - _width));
          break;
        }
        case 5: {
          shape.push_back(QPointF(_width / 2.0, _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  150 * MyMath::sqrt(2.0) - 150 + _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  150 * MyMath::sqrt(2.0) - 150 + _length / 2.0 - _width / 2.0));
          shape.push_back(QPointF(_width / 2.0, _length / 2.0 - _width / 2.0));
          break;
        }
        case 6: {
          shape.push_back(QPointF(_width / 2.0, _length / 2.0 - _width / 2.0));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  150 * MyMath::sqrt(2.0) - 150 + _length / 2.0 - _width / 2.0));
          shape.push_back(QPointF(0, _length / 2.0 + 150 * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(0, _length / 2.0));
          break;
        }
        case 7: {
          shape.push_back(QPointF(-(_width / 2.0), _length / 2.0 - _width / 2.0));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  150 * MyMath::sqrt(2.0) - 150 + _length / 2.0 - _width / 2.0));
          shape.push_back(QPointF(-(0), _length / 2.0 + 150 * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(0), _length / 2.0));
          break;
        }
        case 8: {
          shape.push_back(QPointF(-(_width / 2.0), _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  150 * MyMath::sqrt(2.0) - 150 + _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  150 * MyMath::sqrt(2.0) - 150 + _length / 2.0 - _width / 2.0));
          shape.push_back(QPointF(-(_width / 2.0), _length / 2.0 - _width / 2.0));
          break;
        }
        case 9: {
          shape.push_back(QPointF(-(150 + _width * 3.0 / 2.0),
                                  150 * MyMath::sqrt(2.0) - 150 + _width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  150 * MyMath::sqrt(2.0) - 150 + _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(_width / 2.0), _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(_width * 3.0 / 2.0), _width / MyMath::sqrt(2.0) - _width));
          break;
        }
        case 10: {
          shape.push_back(QPointF(-(150 + _width * 3.0 / 2.0),
                                  -150 * MyMath::sqrt(2.0) - 150 - _width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(-(150 + _width * 3.0 / 2.0),
                                  150 * MyMath::sqrt(2.0) - 150 + _width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(-(_width * 3.0 / 2.0),
                                  _width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(-(_width * 3.0 / 2.0),
                                  -_width / MyMath::sqrt(2.0) - _width));
          break;
        }
        case 11: {
          shape.push_back(
            QPointF(-(150 + _width / 2.0), -150 * MyMath::sqrt(2.0) - 150 - _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(150 + _width * 3.0 / 2.0),
                                  -150 * MyMath::sqrt(2.0) - 150 - _width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(-(_width * 3.0 / 2.0),
                                  -_width / MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(-(_width / 2.0), -_width / MyMath::sqrt(2.0)));
          break;
        }
        case 12: {
          shape.push_back(QPointF(-(150 + _width / 2.0), -150 - _length / 2.0));
          shape.push_back(
            QPointF(-(150 + _width / 2.0), -150 * MyMath::sqrt(2.0) - 150 - _width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(_width / 2.0), -_width / MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(_width / 2.0), -_length / 2.0));
          break;
        }
        default: {
          throw std::range_error("pipe highlight index out of range");
        }
      }
      break;
    }
    case PipeType::output: {
      switch (index) {
        case 0: {
          shape.push_back(QPointF(150 + _width / 2.0, 150 + _length / 2.0));
          shape.push_back(QPointF(_width / 2.0, _length / 2.0));
          shape.push_back(QPointF(-_width / 2.0, _length / 2.0));
          shape.push_back(QPointF(-150 - _width / 2.0, 150 + _length / 2.0));
          break;
        }
        case 1: {
          shape.push_back(
            QPointF(150 + _width / 2.0, -150 * MyMath::sqrt(2.0) + 150 - _length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(150 + _width / 2.0, -150 * MyMath::sqrt(2.0) - 150));
          shape.push_back(QPointF(_width / 2.0, 0));
          shape.push_back(QPointF(_width / 2.0, -_length / 2.0 + _width / 2.0));
          break;
        }
        case 2: {
          shape.push_back(QPointF(150 + _width / 2.0, -150 * MyMath::sqrt(2.0) - 150));
          shape.push_back(QPointF(150 + _width * 3.0 / 2.0,
                                  -150 * MyMath::sqrt(2.0) - 150 - _width));
          shape.push_back(QPointF(_width * 3.0 / 2.0, -_width));
          shape.push_back(QPointF(_width / 2.0, 0));
          break;
        }
        case 3: {
          shape.push_back(QPointF(150 + _width * 3.0 / 2.0,
                                  -150 * MyMath::sqrt(2.0) - 150 - _width));
          shape.push_back(QPointF(150 + _width * 3.0 / 2.0,
                                  150 * MyMath::sqrt(2.0) - 150 + _width * MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(_width * 3.0 / 2.0,
                                  _width * MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(_width * 3.0 / 2.0, -_width));
          break;
        }
        case 4: {
          shape.push_back(QPointF(150 + _width * 3.0 / 2.0,
                                  150 * MyMath::sqrt(2.0) - 150 + _width * MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  150 * MyMath::sqrt(2.0) - 150 + _width * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(_width / 2.0, _width * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(_width * 3.0 / 2.0, _width * MyMath::sqrt(2.0) - _width));
          break;
        }
        case 5: {
          shape.push_back(QPointF(_width / 2.0, _width * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  150 * MyMath::sqrt(2.0) - 150 + _width * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  150 + _length / 2.0));
          shape.push_back(QPointF(_width / 2.0, _length / 2.0));
          break;
        }
        case 6: {
          shape.push_back(QPointF(_width / 2.0, -_length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  -150 * MyMath::sqrt(2.0) + 150 - _length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(0, -_length / 2.0 - 150 * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(0, -_length / 2.0));
          break;
        }
        case 7: {
          shape.push_back(QPointF(-(_width / 2.0), -_length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  -150 * MyMath::sqrt(2.0) + 150 - _length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(-(0), -_length / 2.0 - 150 * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(0), -_length / 2.0));
          break;
        }
        case 8: {
          shape.push_back(QPointF(-(_width / 2.0), _width * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  150 * MyMath::sqrt(2.0) - 150 + _width * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  150 + _length / 2.0));
          shape.push_back(QPointF(-(_width / 2.0), _length / 2.0));
          break;
        }
        case 9: {
          shape.push_back(QPointF(-(150 + _width * 3.0 / 2.0),
                                  150 * MyMath::sqrt(2.0) - 150 + _width * MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  150 * MyMath::sqrt(2.0) - 150 + _width * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(_width / 2.0), _width * MyMath::sqrt(2.0)));
          shape.push_back(QPointF(-(_width * 3.0 / 2.0), _width * MyMath::sqrt(2.0) - _width));
          break;
        }
        case 10: {
          shape.push_back(QPointF(-(150 + _width * 3.0 / 2.0),
                                  -150 * MyMath::sqrt(2.0) - 150 - _width));
          shape.push_back(QPointF(-(150 + _width * 3.0 / 2.0),
                                  150 * MyMath::sqrt(2.0) - 150 + _width * MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(-(_width * 3.0 / 2.0),
                                  _width * MyMath::sqrt(2.0) - _width));
          shape.push_back(QPointF(-(_width * 3.0 / 2.0), -_width));
          break;
        }
        case 11: {
          shape.push_back(QPointF(-(150 + _width / 2.0), -150 * MyMath::sqrt(2.0) - 150));
          shape.push_back(QPointF(-(150 + _width * 3.0 / 2.0),
                                  -150 * MyMath::sqrt(2.0) - 150 - _width));
          shape.push_back(QPointF(-(_width * 3.0 / 2.0), -_width));
          shape.push_back(QPointF(-(_width / 2.0), 0));
          break;
        }
        case 12: {
          shape.push_back(
            QPointF(-(150 + _width / 2.0), -150 * MyMath::sqrt(2.0) + 150 - _length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(-(150 + _width / 2.0), -150 * MyMath::sqrt(2.0) - 150));
          shape.push_back(QPointF(-(_width / 2.0), 0));
          shape.push_back(QPointF(-(_width / 2.0), -_length / 2.0 + _width / 2.0));
          break;
        }
        default: {
          throw std::range_error("pipe highlight index out of range");
        }
      }
      break;
    }
    case PipeType::vertical: {
      switch (index) {
        case 0: {
          shape.push_back(QPointF(0, -150 * MyMath::sqrt(2.0) - _length / 2.0));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(_width / 2.0, -_length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(0, -_length / 2.0));
          break;
        }
        case 1: {
          shape.push_back(QPointF(_width / 2.0, -_length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  -(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0)));
          shape.push_back(QPointF(_width / 2.0, _length / 2.0 - _width / 2.0));
          break;
        }
        case 2: {
          shape.push_back(QPointF(0, 150 * MyMath::sqrt(2.0) + _length / 2.0));
          shape.push_back(QPointF(150 + _width / 2.0,
                                  -(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0)));
          shape.push_back(QPointF(_width / 2.0, _length / 2.0 - _width / 2.0));
          shape.push_back(QPointF(0, _length / 2.0));
          break;
        }
        case 3: {
          shape.push_back(QPointF(0, 150 * MyMath::sqrt(2.0) + _length / 2.0));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  -(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0)));
          shape.push_back(QPointF(-(_width / 2.0), _length / 2.0 - _width / 2.0));
          shape.push_back(QPointF(0, _length / 2.0));
          break;
        }
        case 4: {
          shape.push_back(QPointF(-(_width / 2.0), -_length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  -(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0)));
          shape.push_back(QPointF(-(_width / 2.0), _length / 2.0 - _width / 2.0));
          break;
        }
        case 5: {
          shape.push_back(QPointF(0, -150 * MyMath::sqrt(2.0) - _length / 2.0));
          shape.push_back(QPointF(-(150 + _width / 2.0),
                                  150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(-(_width / 2.0), -_length / 2.0 + _width / 2.0));
          shape.push_back(QPointF(0, -_length / 2.0));
          break;
        }
        default: {
          throw std::range_error("pipe highlight index out of range");
        }
      }
      break;
    }
    case PipeType::horizontal: {
      switch (index) {
        case 0: {
          shape.push_back(QPointF(-150 * MyMath::sqrt(2.0) - _length / 2.0, 0));
          shape.push_back(QPointF(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0,
                                  150 + _width / 2.0));
          shape.push_back(QPointF(-_length / 2.0 + _width / 2.0, _width / 2.0));
          shape.push_back(QPointF(-_length / 2.0, 0));
          break;
        }
        case 1: {
          shape.push_back(QPointF(-_length / 2.0 + _width / 2.0, _width / 2.0));
          shape.push_back(QPointF(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0,
                                  150 + _width / 2.0));
          shape.push_back(QPointF(-(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0),
                                  150 + _width / 2.0));
          shape.push_back(QPointF(_length / 2.0 - _width / 2.0, _width / 2.0));
          break;
        }
        case 2: {
          shape.push_back(QPointF(150 * MyMath::sqrt(2.0) + _length / 2.0, 0));
          shape.push_back(QPointF(-(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0),
                                  150 + _width / 2.0));
          shape.push_back(QPointF(_length / 2.0 - _width / 2.0, _width / 2.0));
          shape.push_back(QPointF(_length / 2.0, 0));
          break;
        }
        case 3: {
          shape.push_back(QPointF(150 * MyMath::sqrt(2.0) + _length / 2.0, 0));
          shape.push_back(QPointF(-(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0),
                                  -(150 + _width / 2.0)));
          shape.push_back(QPointF(_length / 2.0 - _width / 2.0, -(_width / 2.0)));
          shape.push_back(QPointF(_length / 2.0, 0));
          break;
        }
        case 4: {
          shape.push_back(QPointF(-_length / 2.0 + _width / 2.0, -(_width / 2.0)));
          shape.push_back(QPointF(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0,
                                  -(150 + _width / 2.0)));
          shape.push_back(QPointF(-(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0),
                                  -(150 + _width / 2.0)));
          shape.push_back(QPointF(_length / 2.0 - _width / 2.0, -(_width / 2.0)));
          break;
        }
        case 5: {
          shape.push_back(QPointF(-150 * MyMath::sqrt(2.0) - _length / 2.0, 0));
          shape.push_back(QPointF(150 - 150 * MyMath::sqrt(2.0) - _length / 2.0 + _width / 2.0,
                                  -(150 + _width / 2.0)));
          shape.push_back(QPointF(-_length / 2.0 + _width / 2.0, -(_width / 2.0)));
          shape.push_back(QPointF(-_length / 2.0, 0));
          break;
        }
        default: {
          throw std::range_error("pipe highlight index out of range");
        }
      }
      break;
    }
    default: {
      throw std::range_error("PipeType out of range");
    }
  }
  return shape;
}

void PipeHighlight::setHighlightOpacity(qreal ho) {
  _highlightOpacity = ho;
  if (pipeType == PipeType::input or pipeType == PipeType::output) {
    for (int i = 0; i < 13; ++i) {
      highlight[i]->setBrush(getHighlightBrush(i));
    }
  } else {
    for (int i = 0; i < 6; ++i) {
      highlight[i]->setBrush(getHighlightBrush(i));
    }
  }
}

void PipeHighlight::turnOn() {
  delete opacityOnOffAnimation;
  opacityOnOffAnimation = new QPropertyAnimation(this, "highlightOpacity");
  opacityOnOffAnimation->setDuration(250);
  opacityOnOffAnimation->setStartValue(highlightOpacity());
  opacityOnOffAnimation->setEndValue(1);
  opacityOnOffAnimation->start();
  connect(opacityOnOffAnimation, &QPropertyAnimation::finished, [this](){opacityLoopAnimation->start();});
}

void PipeHighlight::turnOff() {
  opacityLoopAnimation->stop();
  delete opacityOnOffAnimation;
  opacityOnOffAnimation = new QPropertyAnimation(this, "highlightOpacity");
  opacityOnOffAnimation->setDuration(250);
  opacityOnOffAnimation->setStartValue(highlightOpacity());
  opacityOnOffAnimation->setEndValue(0);
  opacityOnOffAnimation->start();
}

void PipeHighlight::setSize(int width, int length) {
  _width = width;
  _length = length;
  if (pipeType == PipeType::input or pipeType == PipeType::output) {
    for (int i = 0; i < 13; ++i) {
      highlight[i]->setPolygon(QPolygonF(getHightlightShape(i)));
      highlight[i]->setBrush(getHighlightBrush(i));
    }
  } else {
    for (int i = 0; i < 6; ++i) {
      highlight[i]->setPolygon(QPolygonF(getHightlightShape(i)));
      highlight[i]->setBrush(getHighlightBrush(i));
    }
  }
}
