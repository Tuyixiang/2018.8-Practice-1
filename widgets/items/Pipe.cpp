//
// Created by Yixiang Tu on 29/08/2018.
//

#include "Pipe.h"
#include "../../Chip.h"
#include "../../MyMath.h"
#include "PipeHighlight.h"
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QTimer>

const QColor Pipe::colorDisabled = QColor(220, 220, 220);
const QColor Pipe::colorEnabled = QColor(0, 190, 240);
const QColor Pipe::colorDefunct = QColor(200, 200, 50);
const QColor Pipe::colorInvisible = QColor(255, 255, 255, 0);

Pipe::Pipe(QObject* parent, QGraphicsScene* s, PipeInfo* i) :
  QObject(parent),
  QGraphicsItemGroup(nullptr),
  info(i),
  scene(s) {
  scene->addItem(this);
  connect(this, &Pipe::infoChanged, this, &Pipe::updateToolTip);
  connect(this, &Pipe::propChanged, this, &Pipe::startAnimation);
}

void Pipe::initializePosition() {
  QPointF pos;
  switch (pipeType) {
    case PipeType::input: {
      pos = QPointF(CHIP.accHLength[xIndex], CHIP.accVLength[0] - info->length / 2);
      break;
    }
    case PipeType::output: {
      pos = QPointF(CHIP.accHLength[xIndex], CHIP.accVLength[yIndex] + info->length / 2);
      break;
    }
    case PipeType::vertical: {
      pos = QPointF(CHIP.accHLength[xIndex], CHIP.accVLength[yIndex] + info->length / 2);
      break;
    }
    case PipeType::horizontal: {
      pos = QPointF(CHIP.accHLength[xIndex] + info->length / 2, CHIP.accVLength[yIndex]);
      break;
    }
    default: {
      throw std::range_error("no such PipeType");
    }
  }
  setPos(pos);
  highlight->setPos(pos);
}

void Pipe::initializePolygon(PipeType t, int x, int y) {
  pipeType = t;
  _width = info->width;
  _length = info->length;
  switch (t) {
    case PipeType::input: {
      xIndex = x;
      if (info->disabled) {
        _destColor = _color = colorDisabled;
      } else {
        _destColor = _color = colorEnabled;
      }
      auto brush = getBrush();
      polygon = scene->addPolygon(getShape(), Qt::NoPen, brush);
      addToGroup(polygon);
      break;
    }
    case PipeType::output: {
      xIndex = x;
      yIndex = CHIP.rows - 1;
      if (info->disabled) {
        _destColor = _color = colorDisabled;
      } else {
        _destColor = _color = colorEnabled;
      }
      auto brush = getBrush();
      polygon = scene->addPolygon(getShape(), Qt::NoPen, brush);
      addToGroup(polygon);
      break;
    }
    default: {
      xIndex = x;
      yIndex = y;
      if (info->disabled) {
        _destColor = _color = colorDisabled;
      } else {
        _destColor = _color = colorEnabled;
      }
      auto brush = getBrush();
      polygon = scene->addPolygon(getShape(), Qt::NoPen, brush);
      addToGroup(polygon);
    }
  }
  emit infoChanged();
}

QBrush Pipe::getBrush() {
  QLinearGradient gradient;
  if (pipeType != PipeType::horizontal) {
    gradient.setStart(-_width / 2, 0);
    gradient.setFinalStop(_width / 2, 0);
  } else {
    gradient.setStart(0, -_width / 2);
    gradient.setFinalStop(0, _width / 2);
  }
  gradient.setColorAt(0, _color);
  gradient.setColorAt(0.5, MyMath::colorMultiply(_color, 1.2));
  gradient.setColorAt(1, _color);

  return QBrush(gradient);
}

void Pipe::mousePressEvent(QGraphicsSceneMouseEvent* ev) {
  emit mousePress(this);
}

void Pipe::disabilitySwitch() {
  info->disabled = !info->disabled;
}

void Pipe::startAnimation() {
  checkDestColor();
  if (_color != _destColor) {
    delete colorAnimation;
    colorAnimation = new QPropertyAnimation(this, "color");
    colorAnimation->setDuration(250);
    colorAnimation->setStartValue(_color);
    colorAnimation->setEndValue(_destColor);
    colorAnimation->setEasingCurve(QEasingCurve::OutQuad);
    colorAnimation->start();
  }
  if (sizeAnimation and sizeAnimation->state() == QPropertyAnimation::Running) {
    if (sizeAnimation->endValue() != QSize(info->width, info->length)) {
      sizeAnimation->setStartValue(QSize(_width, _length));
      sizeAnimation->setEndValue(QSize(info->width, info->length));
      sizeAnimation->start();
    }
  } else if (_width != info->width or _length != info->length) {
    delete sizeAnimation;
    sizeAnimation = new QPropertyAnimation(this, "size");
    sizeAnimation->setDuration(500);
    sizeAnimation->setStartValue(QSize(_width, _length));
    sizeAnimation->setEndValue(QSize(info->width, info->length));
    sizeAnimation->start();
  }
}

void Pipe::updateToolTip() {
  QString toolTip("");
  switch (pipeType) {
    case PipeType::input: {
      if (info->disabled) {
        toolTip = "Input disabled\n";
      } else {
        toolTip = "Input\n";
      }
      break;
    }
    case PipeType::output: {
      if (info->disabled) {
        toolTip = "Output disabled\n";
      } else {
        toolTip = "Output\n";
      }
      break;
    }
    default: {
      if (info->disabled) {
        toolTip = "Disabled\n";
      }
      toolTip += "Width: " + QString::number(info->width) +
                 "\nLength: " + QString::number(info->length) + "\n";
    }
  }
  if (CHIP.calculated or pipeType == PipeType::input) {
    toolTip += "Flow rate: " + QString::number(info->rate, 'g', 3) +
               "\nConcentration: " + QString::number(info->concentration, 'g', 3);
  } else {
    toolTip += "Flow rate: -\nConcentration: -";
  }
  polygon->setToolTip(toolTip);
}

Pipe::~Pipe() {
  // remove polygon
}

QVector<QPointF> Pipe::getShape() {
  QVector<QPointF> shape;
  switch (pipeType) {
    case PipeType::input: {
      shape.push_back(QPointF(_width / 2.0, -_length / 2.0));
      shape.push_back(QPointF(_width / 2.0, -_width / MyMath::sqrt(2.0)));
      shape.push_back(QPointF(_width * 3.0 / 2, -_width / MyMath::sqrt(2.0) - _width));
      shape.push_back(QPointF(_width * 3.0 / 2, _width / MyMath::sqrt(2.0) - _width));
      shape.push_back(QPointF(_width / 2.0, _width / MyMath::sqrt(2.0)));
      shape.push_back(QPointF(_width / 2.0, _length / 2.0 - _width / 2.0));
      shape.push_back(QPointF(0, _length / 2.0));
      shape.push_back(QPointF(-_width / 2.0, _length / 2.0 - _width / 2.0));
      shape.push_back(QPointF(-_width / 2.0, _width / MyMath::sqrt(2.0)));
      shape.push_back(QPointF(-_width * 3.0 / 2, _width / MyMath::sqrt(2.0) - _width));
      shape.push_back(QPointF(-_width * 3.0 / 2, -_width / MyMath::sqrt(2.0) - _width));
      shape.push_back(QPointF(-_width / 2.0, -_width / MyMath::sqrt(2.0)));
      shape.push_back(QPointF(-_width / 2.0, -_length / 2.0));
      break;
    }
    case PipeType::output: {
      shape.push_back(QPointF(0, -_length / 2));
      shape.push_back(QPointF(_width / 2.0, (-_length + _width) / 2.0));
      shape.push_back(QPointF(_width / 2.0, 0));
      shape.push_back(QPointF(_width * 3.0 / 2, -_width));
      shape.push_back(QPointF(_width * 3.0 / 2, _width * MyMath::sqrt(2.0) - _width));
      shape.push_back(QPointF(_width / 2.0, _width * MyMath::sqrt(2.0)));
      shape.push_back(QPointF(_width / 2.0, _length / 2.0 - _width / 2.0));
      shape.push_back(QPointF(_width / 2.0, _length / 2.0));
      shape.push_back(QPointF(-_width / 2.0, _length / 2.0));
      shape.push_back(QPointF(-_width / 2.0, _length / 2.0 - _width / 2.0));
      shape.push_back(QPointF(-_width / 2.0, _width * MyMath::sqrt(2.0)));
      shape.push_back(QPointF(-_width * 3.0 / 2, _width * MyMath::sqrt(2.0) - _width));
      shape.push_back(QPointF(-_width * 3.0 / 2, -_width));
      shape.push_back(QPointF(-_width / 2.0, 0));
      shape.push_back(QPointF(-_width / 2.0, (-_length + _width) / 2.0));
      break;
    }
    case PipeType::vertical: {
      shape.push_back(QPointF(0, -_length / 2.0));
      shape.push_back(QPointF(_width / 2.0, (-_length + _width) / 2.0));
      shape.push_back(QPointF(_width / 2.0, (_length - _width) / 2.0));
      shape.push_back(QPointF(0, _length / 2.0));
      shape.push_back(QPointF(-_width / 2.0, (_length - _width) / 2.0));
      shape.push_back(QPointF(-_width / 2.0, (-_length + _width) / 2.0));
      break;
    }
    case PipeType::horizontal: {
      shape.push_back(QPointF(-_length / 2.0, 0));
      shape.push_back(QPointF((-_length + _width) / 2.0, _width / 2.0));
      shape.push_back(QPointF((_length - _width) / 2.0, _width / 2.0));
      shape.push_back(QPointF(_length / 2.0, 0));
      shape.push_back(QPointF((_length - _width) / 2.0, -_width / 2.0));
      shape.push_back(QPointF((-_length + _width) / 2.0, -_width / 2.0));
      break;
    }
    default: {
      throw std::range_error("no such PipeType");
    }
  }
  return shape;
}

void Pipe::checkDestColor() {
  if (!info->visible) {
    if (isVisible()) {
      QTimer::singleShot(250, [&]() {
        hide();
        highlight->hide();
      });
    }
    _destColor = colorInvisible;
  } else {
    if (!isVisible()) {
      show();
      highlight->show();
    }
    if (info->disabled) {
      _destColor = colorDisabled;
    } else if (CHIP.calculated and CHIP.displayType == Chip::Display::rate) {
      if (info->rate) {
        _destColor = MyMath::colorValue(info->rate / CHIP.rateMax);
      } else {
        _destColor = QColor(128, 128, 128);
      }
    } else if (CHIP.calculated and CHIP.displayType == Chip::Display::concentration) {
      if (info->rate) {
        _destColor = MyMath::colorValue(info->concentration / CHIP.concentrationMax);
      } else {
        _destColor = QColor(128, 128, 128);
      }
    } else if (info->defunct) {
      _destColor = colorDefunct;
    } else {
      _destColor = colorEnabled;
    }
  }
}

void Pipe::initializeHighlight() {
  highlight = new PipeHighlight(parent(), scene, this);

}

void Pipe::turnOnHighlight() {
  highlight->turnOn();
}

void Pipe::turnOffHighlight() {
  highlight->turnOff();
}

void Pipe::setSize(const QSize& s) {
  _width = s.width();
  if (_length != s.height()) {
    _length = s.height();
    switch (pipeType) {
      case PipeType::vertical: {
        CHIP.tempVLength[yIndex] = _length;
        break;
      }
      case PipeType::horizontal: {
        CHIP.tempHLength[xIndex] = _length;
        break;
      }
      default: {
      }
    }
  }
  if (_width > _length) {
    _width = _length;
  }
  polygon->setPolygon(getShape());
  polygon->setBrush(getBrush());
  highlight->setSize(_width, _length);
}

void Pipe::calculateTempPosition() {
  QPointF pos;
  switch (pipeType) {
    case PipeType::input: {
      pos = QPointF(CHIP.tempAccHLength[xIndex], CHIP.tempAccVLength[0] - _length / 2);
      break;
    }
    case PipeType::output: {
      pos = QPointF(CHIP.tempAccHLength[xIndex], CHIP.tempAccVLength[yIndex] + _length / 2);
      break;
    }
    case PipeType::vertical: {
      pos = QPointF(CHIP.tempAccHLength[xIndex], CHIP.tempAccVLength[yIndex] + _length / 2);
      break;
    }
    case PipeType::horizontal: {
      pos = QPointF(CHIP.tempAccHLength[xIndex] + _length / 2, CHIP.tempAccVLength[yIndex]);
      break;
    }
    default: {
      throw std::range_error("no such PipeType");
    }
  }
  setPos(pos);
  highlight->setPos(pos);
}

QPointF Pipe::realPos() const {
  switch (pipeType) {
    case PipeType::input: {
      return QPointF(CHIP.accHLength[xIndex], CHIP.accVLength[0] - info->length / 2);
    }
    case PipeType::output: {
      return QPointF(CHIP.accHLength[xIndex], CHIP.accVLength[yIndex] + info->length / 2);
    }
    case PipeType::vertical: {
      return QPointF(CHIP.accHLength[xIndex], CHIP.accVLength[yIndex] + info->length / 2);
    }
    case PipeType::horizontal: {
      return QPointF(CHIP.accHLength[xIndex] + info->length / 2, CHIP.accVLength[yIndex]);
    }
    default: {
      throw std::range_error("no such PipeType");
    }
  }
}

