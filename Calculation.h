//
// Created by Yixiang Tu on 02/09/2018.
//

#ifndef CHIP_SIMULATOR_CALCULATION_H
#define CHIP_SIMULATOR_CALCULATION_H

#include <QVector>

QVector<double> chipCalculation(const QVector<double>& channels, int n, const QVector<int>& io);

QVector<double> getConcentration(double c1, double c2);

#endif //CHIP_SIMULATOR_CALCULATION_H
