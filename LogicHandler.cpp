//
// Created by Yixiang Tu on 30/08/2018.
//

#include "LogicHandler.h"
#include "widgets/fields/ControlPanel.h"
#include "Chip.h"
#include "MyMath.h"
#include "widgets/fields/MainWindow.h"
#include "widgets/fields/Display.h"
#include "widgets/fields/FocusView.h"
#include "Calculation.h"
#include <QDebug>
#include <QDialog>
#include <QErrorMessage>
#include <QTimer>

LogicHandler::LogicHandler(QObject* parent) :
  QObject(parent), mainWindow(dynamic_cast<MainWindow*>(parent)) {}

void LogicHandler::controlPanelEvent(const ControlPanelEvent& ev) {
  switch (ev.type) {
    case ControlPanelEvent::EventType::disabilitySwitch: {
      emit switchDisability(ev.target);
      emit requireChipReexamine();
      CHIP.calculated = false;
      emit requireDisplayRedraw();
      break;
    }
    case ControlPanelEvent::EventType::randomizeMap: {
      //if (currentFocus) {
      //  emit turnOffHighlight(currentFocus);
      //}
      //emit setFocus(nullptr);
      CHIP.randomize();
      emit requireDisplayRedraw();
      break;
    }
    case ControlPanelEvent::EventType::pipeChange: {
      if (ev.rate) {
        ev.target->info->rate = ev.rate;
        ev.target->info->concentration = ev.concentration;
      }
      if (ev.target->info->length != ev.length) {
        CHIP.setPipeInfo(ev.target, ev.width, ev.length);
        emit setFocus(ev.target);
      } else {
        CHIP.setPipeInfo(ev.target, ev.width, ev.length);
      }
      emit requireDisplayRedraw();
      break;
    }
    case ControlPanelEvent::EventType::badInput: {
      static QErrorMessage* message = nullptr;
      if (!message) {
        message = QErrorMessage::qtHandler();
      }
      message->showMessage(ev.message);
      break;
    }
    case ControlPanelEvent::EventType::randomizePipeSize: {
      CHIP.randomizeSize();
      emit requireDisplayRedraw();
      emit setFocus(currentFocus);
      break;
    }
    case ControlPanelEvent::EventType::clearPipes: {
      for (auto* p: mainWindow->display->pipes()) {
        if (p->info->defunct) {
          p->info->disabled = true;
          p->info->defunct = false;
        }
      }
      emit requireDisplayRedraw();
      break;
    }
    case ControlPanelEvent::EventType::reset: {
      for (auto* p: mainWindow->display->pipes()) {
        p->turnOffHighlight();
      }
      CHIP.initializeChip();
      CHIP.markSizeChanging();
      emit setFocus(currentFocus = nullptr);
      emit requireDisplayRedraw();
      emit reset();
      break;
    }
    case ControlPanelEvent::EventType::chipChange: {
      if (ev.width != CHIP.rows or ev.length != CHIP.cols) {
        if (ev.width != CHIP.rows) {
          for (auto* p: mainWindow->display->outPipe) {
            p->_color = Pipe::colorInvisible;
          }
        }
        CHIP.setSize(ev.width, ev.length);
        if (currentFocus) {
          emit turnOffHighlight(currentFocus);
        }
        emit setFocus(currentFocus = nullptr);
        emit requireChipReexamine();
        emit requireDisplayRedraw();
      }
      break;
    }
    case ControlPanelEvent::EventType::calculate: {
      static QErrorMessage* message = nullptr;
      if (CHIP.rows != CHIP.cols) {
        if (!message) {
          message = QErrorMessage::qtHandler();
        }
        message->showMessage("Cannot calculate: rows != cols");
        break;
      }
      QVector<double> channels;
      QVector<int> io;
      for (int i = 0; i < CHIP.rows; ++i) {
        for (int j = 0; j < CHIP.rows - 1; ++j) {
          if (CHIP.vPipe[i][j].disabled) {
            channels.push_back(0);
          } else {
            channels.push_back((CHIP.vPipe[i][j].length - 200.0) / 8.0 /
                               CHIP.vPipe[i][j].width);
          }
        }
      }
      for (int i = 0; i < CHIP.rows - 1; ++i) {
        for (int j = 0; j < CHIP.rows; ++j) {
          if (CHIP.hPipe[i][j].disabled) {
            channels.push_back(0);
          } else {
            channels.push_back((CHIP.hPipe[i][j].length - 200.0) / 8.0 /
                               CHIP.hPipe[i][j].width);
          }
        }
      }
      for (int i = 0; i < 5; ++i) {
        channels.push_back(1);
      }
      double c[2] = {-1};
      for (int i = 0; i < CHIP.rows; ++i) {
        if (!CHIP.inPipe[i].disabled) {
          io.push_back(i);
          c[c[0] != -1] = CHIP.inPipe[i].rate;
        }
      }
      if (io.size() != 2) {
        if (!message) {
          message = QErrorMessage::qtHandler();
        }
        message->showMessage("Cannot calculate: need exactly 2 inputs");
        break;
      }
      if (c[0] != c[1]) {
        if (!message) {
          message = QErrorMessage::qtHandler();
        }
        message->showMessage("Cannot calculate: 2 input rates must be equal");
        break;
      }
      for (int i = 0; i < CHIP.rows; ++i) {
        if (!CHIP.outPipe[i].disabled) {
          io.push_back(i);
        }
      }
      if (io.size() != 5) {
        if (!message) {
          message = QErrorMessage::qtHandler();
        }
        message->showMessage("Cannot calculate: need exactly 3 outputs");
        break;
      }
      //qDebug() << channels;
      auto result = chipCalculation(channels, CHIP.rows, io);
      auto res = getConcentration(CHIP.inPipe[io[0]].concentration, CHIP.inPipe[io[1]].concentration);
      CHIP.concentrationMax = CHIP.inPipe[io[0]].concentration > CHIP.inPipe[io[1]].concentration ? CHIP.inPipe[io[0]].concentration : CHIP.inPipe[io[1]].concentration;
      for (auto& v: res) {
        CHIP.concentrationMax = CHIP.concentrationMax > v ? CHIP.concentrationMax : v;
      }
      //qDebug() << CHIP.concentrationMax;
      for (auto& v: result) {
        v *= c[0] / 200;
      }
      int cnt = 0;
      for (int i = 0; i < CHIP.rows; ++i) {
        for (int j = 0; j < CHIP.rows - 1; ++j) {
          if (!CHIP.vPipe[i][j].disabled) {
            CHIP.vPipe[i][j].rate = result[cnt];
          }
          CHIP.vPipe[i][j].concentration = res[cnt];
          ++cnt;
        }
      }
      for (int i = 0; i < CHIP.rows - 1; ++i) {
        for (int j = 0; j < CHIP.rows; ++j) {
          if (!CHIP.hPipe[i][j].disabled) {
            CHIP.hPipe[i][j].rate = result[cnt];
          }
          CHIP.hPipe[i][j].concentration = res[cnt];
          ++cnt;
        }
      }
      ++cnt;
      for (int i = 2; i < 5; ++i) {
        CHIP.outPipe[io[i]].rate = result[++cnt];
        CHIP.outPipe[io[i]].concentration = res[cnt];
      }
      CHIP.rateMax = 0;
      result.push_back(CHIP.inPipe[io[0]].rate);
      result.push_back(CHIP.inPipe[io[1]].rate);
      for (auto e: result) {
        CHIP.rateMax = CHIP.rateMax > e ? CHIP.rateMax : e;
      }
      CHIP.displayType = Chip::Display::rate;
      CHIP.calculated = true;
      emit requireDisplayRedraw();
    }
  }
}

void LogicHandler::pipePressed(Pipe* target) {
  switch (activeTool) {
    case Tools::select: {
      if (currentFocus != target) {
        if (currentFocus) {
          emit turnOffHighlight(currentFocus);
        }
        currentFocus = target;
        emit turnOnHighlight(currentFocus);
        emit setFocus(currentFocus);
      } else {
        emit switchDisability(target);
        emit requireChipReexamine();
        emit requireDisplayRedraw();
      }
      break;
    }
    case Tools::add: {
      if (target->info->disabled) {
        emit switchDisability(target);
        emit requireChipReexamine();
        emit setFocus(target);
        currentFocus = target;
        emit requireDisplayRedraw();
      }
      break;
    }
    case Tools::remove: {
      if (!target->info->disabled) {
        emit switchDisability(target);
        emit requireChipReexamine();
        emit setFocus(target);
        currentFocus = target;
        emit requireDisplayRedraw();
      }
      break;
    }
  }
  mouseDown = true;
}

void LogicHandler::pipeRightClicked(Pipe* target) {
  pipePressed(target);
  /*if (currentFocus != target) {
    if (currentFocus) {
      emit turnOffHighlight(currentFocus);
    }
    currentFocus = target;
    emit turnOnHighlight(currentFocus);
    emit setFocus(currentFocus);
  }
  auto popup = new ChipSizePopup();
  popup->exec();*/
}

void LogicHandler::defaultPressed() {
  if (currentFocus) {
    emit turnOffHighlight(currentFocus);
    currentFocus = nullptr;
    emit setFocus(nullptr);
  }
  mouseDown = true;
}

void LogicHandler::defaultMouseMove() {
  switch (activeTool) {
    case Tools::select: {
      mainWindow->display->setCursor(Qt::ArrowCursor);
      mainWindow->controlPanel->focusView->setCursor(Qt::ArrowCursor);
      break;
    }
    default: {
    }
  }
}

void LogicHandler::pipeMouseMove(Pipe* target) {
  switch (activeTool) {
    case Tools::select: {
      mainWindow->display->setCursor(Qt::PointingHandCursor);
      mainWindow->controlPanel->focusView->setCursor(Qt::PointingHandCursor);
      break;
    }
    case Tools::add: {
      if (mouseDown) {
        if (target->info->disabled) {
          emit switchDisability(target);
          emit requireChipReexamine();
          emit setFocus(target);
          currentFocus = target;
          emit requireDisplayRedraw();
        }
      }
      break;
    }
    case Tools::remove: {
      if (mouseDown) {
        if (!target->info->disabled) {
          emit switchDisability(target);
          emit requireChipReexamine();
          emit setFocus(target);
          currentFocus = target;
          emit requireDisplayRedraw();
        }
      }
      break;
    }
  }
}

void LogicHandler::released() {
  mouseDown = false;
}

void LogicHandler::toolSwitch(Tools t) {
  if (activeTool != t) {
    activeTool = t;
    switch (activeTool) {
      case Tools::select: {
        mainWindow->display->setCursor(Qt::ArrowCursor);
        mainWindow->controlPanel->focusView->setCursor(Qt::ArrowCursor);
        //emit setFocusViewTool(Tools::select);
        break;
      }
      case Tools::add: {
        mainWindow->display->setCursor(Qt::DragCopyCursor);
        mainWindow->controlPanel->focusView->setCursor(Qt::DragCopyCursor);
        //emit setFocusViewTool(Tools::add);
        break;
      }
      case Tools::remove: {
        mainWindow->display->setCursor(Qt::ForbiddenCursor);
        mainWindow->controlPanel->focusView->setCursor(Qt::ForbiddenCursor);
        //emit setFocusViewTool(Tools::remove);
        break;
      }
    }
  }
}

