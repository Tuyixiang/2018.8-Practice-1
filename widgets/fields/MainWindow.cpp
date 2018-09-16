//
// Created by Yixiang Tu on 28/08/2018.
//

#include "MainWindow.h"
#include "Display.h"
#include "ControlPanel.h"
#include "Toolkit.h"
#include "../../LogicHandler.h"
#include "../items/Pipe.h"
#include "FocusView.h"
#include "../items/Scene.h"
#include "FocusViewDescriptionBox.h"
#include "GlobalSettingsBox.h"
#include "ChipSizePopup.h"
#include <QLayout>
#include <QMenuBar>
#include <QKeyEvent>
#include <QMenu>
#include <QDebug>
#include <QPropertyAnimation>

MainWindow::MainWindow() : QMainWindow() {
  setWindowTitle("Microfluidic Chip Simulator");
  setFixedSize(1171, 600);

  initializeMenuBar();

  logicHandler = new LogicHandler(this);

  widget = new QWidget(this);
  auto layout = new QHBoxLayout(widget);
  toolkit = new Toolkit(this);
  layout->addWidget(toolkit);
  display = new Display(this);
  layout->addWidget(display);
  controlPanel = new ControlPanel(this);
  layout->addWidget(controlPanel);

  controlPanel->setScene(display->scene);

  connect(controlPanel, &ControlPanel::controlPanelEvent, logicHandler, &LogicHandler::controlPanelEvent);
  connect(toolkit, &Toolkit::toolSwitch, logicHandler, &LogicHandler::toolSwitch);
  connect(display->scene, &Scene::pipeClick, logicHandler, &LogicHandler::pipePressed);
  connect(display->scene, &Scene::pipeRightClick, logicHandler, &LogicHandler::pipeRightClicked);
  connect(display->scene, &Scene::defaultClick, logicHandler, &LogicHandler::defaultPressed);
  connect(display->scene, &Scene::pipeMouseMove, logicHandler, &LogicHandler::pipeMouseMove);
  connect(display->scene, &Scene::defaultMouseMove, logicHandler, &LogicHandler::defaultMouseMove);
  connect(display->scene, &Scene::mouseRelease, logicHandler, &LogicHandler::released);
  connect(logicHandler, &LogicHandler::requireDisplayRedraw, display, &Display::startAnimation);
  connect(logicHandler, &LogicHandler::requireDisplayRedraw, controlPanel->focusViewDescriptionBox,
          &FocusViewDescriptionBox::updateInfo);
  connect(logicHandler, &LogicHandler::setFocus, controlPanel->focusView, &FocusView::focusOnPipe);
  connect(logicHandler, &LogicHandler::setFocus, controlPanel->focusViewDescriptionBox,
          &FocusViewDescriptionBox::pipeSelected);
  connect(logicHandler, &LogicHandler::setFocusViewTool, controlPanel->focusView, &FocusView::setTool);
  connect(logicHandler, &LogicHandler::turnOnHighlight, display, &Display::turnOnHighlight);
  connect(logicHandler, &LogicHandler::turnOffHighlight, display, &Display::turnOffHighlight);
  connect(logicHandler, &LogicHandler::switchDisability, display, &Display::switchDisability);
  connect(logicHandler, &LogicHandler::requireChipReexamine, &CHIP, &Chip::checkDefunct);
  connect(logicHandler, &LogicHandler::reset, display, &Display::resetFocus);
  connect(logicHandler, &LogicHandler::requireDisplayRedraw, controlPanel->globalSettingsBox, &GlobalSettingsBox::updateSettings);

  widget->setLayout(layout);
  setCentralWidget(widget);

  setMouseTracking(true);

}

MainWindow::~MainWindow() {

}

void MainWindow::initializeMenuBar() {
  auto menuFile = menuBar()->addMenu("File");
  connect(menuFile->addAction("New"), &QAction::triggered, [&]() {
    auto popup = new ChipSizePopup();
    popup->exec();
    if (popup->completed) {
      auto* result = popup->result;
      logicHandler->controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::reset));
      for (int i = 2; i < 4; ++i) {
        CHIP.inPipe[result[i]].disabled = false;
        CHIP.inPipe[result[i]].defunct = true;
      }
      CHIP.inPipe[result[2]].concentration = 1;
      CHIP.inPipe[result[3]].concentration = 0;
      for (int i = 4; i < 7; ++i) {
        CHIP.outPipe[result[i]].disabled = false;
        CHIP.outPipe[result[i]].defunct = true;
      }
      logicHandler->controlPanelEvent(ControlPanelEvent(result[0], result[1]));
      display->startAnimation();
    }
  });
  auto menuView = menuBar()->addMenu("Show");
  connect(menuView->addAction("Normal"), &QAction::triggered, [&]() {
    CHIP.displayType = Chip::Display::normal;
    display->startAnimation();
  });
  connect(menuView->addAction("Flow Rate"), &QAction::triggered, [&]() {
    if (CHIP.calculated) {
      CHIP.displayType = Chip::Display::rate;
      display->startAnimation();
    } else {
      logicHandler->controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::calculate));
    }
  });
  connect(menuView->addAction("Concentration"), &QAction::triggered, [&]() {
    if (CHIP.calculated) {
      CHIP.displayType = Chip::Display::concentration;
      display->startAnimation();
    } else {
      logicHandler->controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::calculate));
      CHIP.displayType = Chip::Display::concentration;
      display->startAnimation();
    }
  });
}
