//
// Created by Yixiang Tu on 29/08/2018.
//

#ifndef CHIP_SIMULATOR_TOOLKIT_H
#define CHIP_SIMULATOR_TOOLKIT_H
#include "../../EnumTypes.h"
#include <QGraphicsView>

class ToolkitButton;

class Toolkit : public QGraphicsView {

  Q_OBJECT

signals:
  void toolSwitch(Tools);

public:
  explicit Toolkit(QWidget* parent);
  QSize sizeHint() const override {
    return QSize(80, 576);
  }

protected:
  void mouseMoveEvent(QMouseEvent* ev) override;

  QGraphicsScene* scene = nullptr;
  ToolkitButton* selectButton = nullptr;
  ToolkitButton* addButton = nullptr;
  ToolkitButton* removeButton = nullptr;

};


#endif //CHIP_SIMULATOR_TOOLKIT_H
