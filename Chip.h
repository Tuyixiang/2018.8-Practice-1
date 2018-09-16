//
// Created by Yixiang Tu on 29/08/2018.
//

#ifndef CHIP_SIMULATOR_CHIP_H
#define CHIP_SIMULATOR_CHIP_H

#include <QObject>

#define CHIP Chip::instance()

class Pipe;

struct PipeInfo {
  bool disabled = true;
  bool defunct = false;
  bool visible = true;
  int width = 200;
  double rate = 0;
  double concentration = 0;
  int length = 1800;
};

class Chip : public QObject {

Q_OBJECT

  friend class ChipDisplay;

  friend class Pipe;

public:
  static Chip& instance(const char* reset = nullptr) {
    static Chip chip;
    if (reset) {
      chip.initializeChip();
    }
    return chip;
  }

  /// whether a calculation is done since the last change
  bool calculated = false;
  qreal rateMax = 1;
  qreal concentrationMax = 1;

  enum class Display {
    normal,
    rate,
    concentration
  } displayType = Display::normal;

  /// whether a size-changing animation is on
  int sizeChanging = 0;

  int rows = 8;
  int cols = 8;

  /// below are the values set by the user
  int vLength[7];
  int accVLength[8];
  int hLength[7];
  int accHLength[8];

  /// below are the temporary values on display, possibly during an animation
  int tempVLength[7];
  int tempAccVLength[8];
  int tempHLength[7];
  int tempAccHLength[8];

  PipeInfo inPipe[8], outPipe[8], vPipe[8][7], hPipe[7][8];

  void calcAccLengths();

  void calcTempAccLengths();

  void randomize();

  void randomizeSize();

  bool validateInputs(Pipe* target, int width, int length);

  void setPipeInfo(Pipe* target, int width, int length);

  void initializeChip();

  void markSizeChanging();

  void setSize(int r, int c);

signals:

  void chipChanged();

public slots:

  void checkDefunct();

protected:

  Chip();

  Chip(const Chip& other) = delete;

  Chip& operator=(const Chip& other) = delete;

};


#endif //CHIP_SIMULATOR_CHIP_H
