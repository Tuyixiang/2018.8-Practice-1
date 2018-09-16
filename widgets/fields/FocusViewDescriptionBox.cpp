//
// Created by Yixiang Tu on 01/09/2018.
//

#include "FocusViewDescriptionBox.h"
#include "../items/Pipe.h"
#include "../../MyMath.h"
#include "ControlPanel.h"
#include "../items/TextInput.h"
#include <QLayout>
#include <QLabel>
#include <QFormLayout>

FocusViewDescriptionBox::FocusViewDescriptionBox(QWidget* parent) : QWidget(parent) {
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 10, 0, 10);
  layout->setSpacing(20);
  layout->setAlignment(Qt::AlignLeft);
  setLayout(layout);


  disabilityButton = new QPushButton(this);
  disabilityButton->setFont(QFont("Helvetica", 15));
  layout->addWidget(disabilityButton);
  {
    disabilityButton->setText("nothing selected");
    disabilityButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    disabilityButton->setEnabled(false);
    connect(disabilityButton, &QPushButton::clicked, [&]() {
      if (selectedPipe)
        emit controlPanelEvent(
          ControlPanelEvent(selectedPipe, ControlPanelEvent::EventType::disabilitySwitch));
    });
  }

  auto inputField = new QWidget(this);
  layout->addWidget(inputField);
  {
    auto inputFieldLayout = new QHBoxLayout(inputField);
    inputField->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    inputFieldLayout->setAlignment(Qt::AlignVCenter);
    inputFieldLayout->setContentsMargins(0, 0, 0, 0);
    inputField->setLayout(inputFieldLayout);

    auto inputs = new QWidget(inputField);
    inputFieldLayout->addWidget(inputs);
    {
      auto inputsLayout = new QFormLayout(inputs);
      inputsLayout->setContentsMargins(0, 0, 8, 0);
      inputs->setLayout(inputsLayout);
      widthInput = new TextInput(inputs);
      inputsLayout->addRow(new QLabel("width", inputField), widthInput);
      {
        widthInput->setText("-");
        widthInput->setEnabled(false);
        connect(widthInput, &TextInput::inputComplete, this, &FocusViewDescriptionBox::submitChanges);
      }
      lengthInput = new TextInput(inputs);
      inputsLayout->addRow(new QLabel("length", inputField), lengthInput);
      {
        lengthInput->setText("-");
        lengthInput->setEnabled(false);
        connect(lengthInput, &TextInput::inputComplete, this, &FocusViewDescriptionBox::submitChanges);
      }
      rateInput = new TextInput(inputs);
      inputsLayout->addRow(new QLabel("rate", inputField), rateInput);
      {
        rateInput->setText("-");
        rateInput->setEnabled(false);
        connect(rateInput, &TextInput::inputComplete, this, &FocusViewDescriptionBox::submitChanges);
      }
      concentrationInput = new TextInput(inputs);
      inputsLayout->addRow(new QLabel("conc.", inputField), concentrationInput);
      {
        concentrationInput->setText("-");
        concentrationInput->setEnabled(false);
        connect(concentrationInput, &TextInput::inputComplete, this, &FocusViewDescriptionBox::submitChanges);
      }

      inputCompleteButton = new QPushButton(inputField);
      inputFieldLayout->addWidget(inputCompleteButton);
      {
        inputCompleteButton->setText("change");
        inputCompleteButton->setFont(QFont("Helvetica", 15));
        inputCompleteButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        inputCompleteButton->setEnabled(false);
        connect(inputCompleteButton, &QPushButton::clicked, this, &FocusViewDescriptionBox::submitChanges);
      }
    }

  }


}

void FocusViewDescriptionBox::pipeSelected(Pipe* target) {
  if (target) {
    disabilityButton->setEnabled(true);
    QString disabilityButtonText;
    switch (target->pipeType) {
      case PipeType::input: {
        widthInput->setEnabled(false);
        lengthInput->setEnabled(false);
        inputCompleteButton->setEnabled(false);
        disabilityButtonText = "input";
        break;
      }
      case PipeType::output: {
        widthInput->setEnabled(false);
        lengthInput->setEnabled(false);
        inputCompleteButton->setEnabled(false);
        disabilityButtonText = "output";
        break;
      }
      default: {
        widthInput->setEnabled(true);
        lengthInput->setEnabled(true);
        inputCompleteButton->setEnabled(true);
        disabilityButtonText = "channel";
      }
    }
    if (target->info->disabled) {
      disabilityButtonText += " disabled";
    } else {
      disabilityButtonText += " enabled";
    }
    disabilityButton->setText(disabilityButtonText);
    widthInput->setText(QString::number(target->info->width));
    lengthInput->setText(QString::number(target->info->length - 200));
    if (target->pipeType == PipeType::input) {
      rateInput->setText(QString::number(target->info->rate, 'g', 3));
      concentrationInput->setText(QString::number(target->info->concentration, 'g', 3));
      rateInput->setEnabled(true);
      concentrationInput->setEnabled(true);
    } else {
      rateInput->setEnabled(false);
      concentrationInput->setEnabled(false);
      if (CHIP.calculated) {
        rateInput->setText(QString::number(target->info->rate, 'g', 3));
        concentrationInput->setText(QString::number(target->info->concentration, 'g', 3));
      } else {
        rateInput->setText("-");
        concentrationInput->setText("-");
      }
    }
    inputCompleteButton->setEnabled(true);
  } else {
    disabilityButton->setText("nothing selected");
    disabilityButton->setEnabled(false);
    widthInput->setText("-");
    widthInput->setEnabled(false);
    lengthInput->setText("-");
    lengthInput->setEnabled(false);
    inputCompleteButton->setEnabled(false);
  }
  selectedPipe = target;
  disabilityButton->repaint();
}

void FocusViewDescriptionBox::updateInfo() {
  pipeSelected(selectedPipe);
}

void FocusViewDescriptionBox::submitChanges() {
  int width = widthInput->text().toInt();
  int length = lengthInput->text().toInt() + 200;
  qreal rate = 0;
  qreal concentration = 0;
  if (CHIP.validateInputs(selectedPipe, width, length)) {
    if (selectedPipe->pipeType == PipeType::input) {
      rate = rateInput->text().toDouble();
      concentration = concentrationInput->text().toDouble();
      if (rate <= 0 or concentration < 0) {
        emit controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::badInput,
                                                 "Invalid rate or concentration!"));
        updateInfo();
      } else if (selectedPipe->info->width != width or
                 selectedPipe->info->length != length or
                 MyMath::abs(selectedPipe->info->rate - rate) > 0.0001 or
                 MyMath::abs(selectedPipe->info->concentration != concentration) > 0.0001) {
        emit controlPanelEvent(ControlPanelEvent(selectedPipe, width, length, rate, concentration));
      }
    } else if (selectedPipe->info->width != width or selectedPipe->info->length != length) {
      emit controlPanelEvent(ControlPanelEvent(selectedPipe, width, length));
    }
  } else {
    emit controlPanelEvent(ControlPanelEvent(ControlPanelEvent::EventType::badInput,
                                             "Invalid width or length!"));
    updateInfo();
  }
}
