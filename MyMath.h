//
// Created by Yixiang Tu on 30/08/2018.
//

#ifndef CHIP_SIMULATOR_MATH_H
#define CHIP_SIMULATOR_MATH_H

#include <math.h>
#include <QRandomGenerator>
#include <QRectF>

class QColor;

namespace MyMath {

  template<typename T>
  const T& max(const T& a, const T& b);

  template<typename T>
  const T& min(const T& a, const T& b);

  /**
   * @param c alpha will be ignored
   * @param amount range [0,1]
   * @return a new color blended with <b>amount</b> of white
   */
  QColor colorBlendWithWhite(const QColor& c, double amount);

  /**
   * multiply each channel by <b>x</b>, maximum 255<br>
   * preserves alpha
   */
  QColor colorMultiply(const QColor& c, double x);

  QColor colorValue(qreal percent);

  QRectF rectCenterProportion(const QRectF& rect, qreal w, qreal h);

  QRectF rectFromCenter(qreal x, qreal y, qreal w, qreal h);

  QRectF rectFromCenter(const QPointF& p, qreal w, qreal h);

  double sqrt(double x);

  double abs(double x);

  extern QRandomGenerator ran;

};


#endif //CHIP_SIMULATOR_MATH_H
