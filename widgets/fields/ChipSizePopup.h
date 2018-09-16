//
// Created by Yixiang Tu on 02/09/2018.
//

#ifndef CHIP_SIMULATOR_CHIPSIZEPOPUP_H
#define CHIP_SIMULATOR_CHIPSIZEPOPUP_H


#include <QDialog>
#include <QLineEdit>

class ChipSizePopup : public QDialog {

  Q_OBJECT

public:
  ChipSizePopup();

  QLineEdit* input[7];
  int result[7];
  bool correct[7];
  bool completed = false;
};


#endif //CHIP_SIMULATOR_CHIPSIZEPOPUP_H
