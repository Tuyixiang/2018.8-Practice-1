//
// Created by Yixiang Tu on 30/08/2018.
//

#include "MyMath.h"
#include "Chip.h"
#include <cmath>
#include <QColor>

QRandomGenerator MyMath::ran = QRandomGenerator::securelySeeded();

QColor MyMath::colorBlendWithWhite(const QColor& c, double amount) {
  return QColor(static_cast<int>(c.red() * amount + 255 * (1 - amount)),
                static_cast<int>(c.green() * amount + 255 * (1 - amount)),
                static_cast<int>(c.blue() * amount + 255 * (1 - amount)));
}

QColor MyMath::colorMultiply(const QColor& c, double x) {
  return QColor(min(255, static_cast<int>(c.red() * x)),
                min(255, static_cast<int>(c.green() * x)),
                min(255, static_cast<int>(c.blue() * x)),
                c.alpha());
}

template<typename T>
const T& MyMath::max(const T& a, const T& b) {
  return a > b ? a : b;
}

template<typename T>
const T& MyMath::min(const T& a, const T& b) {
  return a > b ? b : a;
}

double MyMath::sqrt(double x) {
  return std::sqrt(x);
}

QRectF MyMath::rectCenterProportion(const QRectF& rect, qreal w, qreal h) {
  return QRectF(rect.x() + rect.width() * (1 - w) / 2,
                rect.y() + rect.height() * (1 - h) / 2,
                rect.width() * w,
                rect.height() * h);
}

QRectF MyMath::rectFromCenter(qreal x, qreal y, qreal w, qreal h) {
  return QRectF(x - w / 2, y - h / 2, w, h);
}

QRectF MyMath::rectFromCenter(const QPointF& p, qreal w, qreal h) {
  return rectFromCenter(p.x(), p.y(), w, h);
}

double MyMath::abs(double x) {
  return x > 0 ? x : -x;
}

QColor MyMath::colorValue(qreal percent) {

  percent = sqrt(percent);
  if (percent < 0.5) {
    return QColor(static_cast<int>(510 * percent), 255, 0);
  } else {
    return QColor(255, static_cast<int>(510 - 510 * percent), 0);
  }

}
