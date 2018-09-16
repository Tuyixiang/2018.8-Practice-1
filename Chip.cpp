//
// Created by Yixiang Tu on 29/08/2018.
//

#include "Chip.h"
#include "MyMath.h"
#include "EnumTypes.h"
#include "widgets/items/Pipe.h"
#include <QQueue>
#include <QDebug>
#include <iostream>
#include <QTimer>

template<typename T>
struct Triplet {
  T a, b, c;

  Triplet(const T& _a, const T& _b, const T& _c = T()) {
    a = _a;
    b = _b;
    c = _c;
  }
};

void Chip::calcAccLengths() {
  accVLength[0] = accHLength[0] = 0;
  for (int i = 0; i < 7; ++i) {
    accVLength[i + 1] = accVLength[i] + vLength[i];
    accHLength[i + 1] = accHLength[i] + hLength[i];
  }
}

void Chip::calcTempAccLengths() {
  tempAccVLength[0] = tempAccHLength[0] = 0;
  for (int i = 0; i < 7; ++i) {
    if (i < rows - 1) {
      tempAccVLength[i + 1] = tempAccVLength[i] + tempVLength[i];
    } else {
      tempAccVLength[i + 1] = tempAccVLength[i];
    }
    tempAccHLength[i + 1] = tempAccHLength[i] + tempHLength[i];
  }
}

Chip::Chip() {
  initializeChip();
}

void Chip::randomize() {
  for (int i = 0; i < cols; ++i) {
    inPipe[i].disabled = true;
    outPipe[i].disabled = true;
  }
  inPipe[MyMath::ran.bounded(cols)].disabled = false;
  int a;
  while (!inPipe[a = MyMath::ran.bounded(cols)].disabled);
  inPipe[a].disabled = false;
  for (int i = 0; i < cols; ++i) {
    if (!inPipe[i].disabled) {
      inPipe[i].concentration = 1;
      break;
    }
  }

  outPipe[MyMath::ran.bounded(cols)].disabled = false;
  while (!outPipe[a = MyMath::ran.bounded(cols)].disabled);
  outPipe[a].disabled = false;
  while (!outPipe[a = MyMath::ran.bounded(cols)].disabled);
  outPipe[a].disabled = false;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 7; ++j) {
      vPipe[i][j].disabled = !(i < cols and j < rows - 1 and MyMath::ran.bounded(10));
      hPipe[j][i].disabled = !(i < rows and j < cols - 1 and MyMath::ran.bounded(10));
    }
  }
  calculated = false;
  checkDefunct();
}

void Chip::randomizeSize() {
  for (int i = 0; i < 7; ++i) {
    //vLength[i] = MyMath::ran.bounded(1400, 2000);
    //hLength[i] = MyMath::ran.bounded(1400, 2000);
    calcAccLengths();
    for (int j = 0; j < 8; ++j) {
      //vPipe[j][i].length = vLength[i];
      vPipe[j][i].width = MyMath::ran.bounded(100, 400);
      //hPipe[i][j].length = hLength[i];
      hPipe[i][j].width = MyMath::ran.bounded(100, 400);
    }
  }
  //markSizeChanging();
  calculated = false;
}

void Chip::checkDefunct() {

  /// mark defunct pipes that are not connect to both an input and an output
  {
    bool connectedToInput[8][8];
    bool connectedToOutput[8][8];
    bool connectedToInputFrom[8][8][4];
    bool connectedToOutputFrom[8][8][4];
    QQueue<Triplet<int> > searchQueue;
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        connectedToInput[i][j] = connectedToOutput[i][j] = false;
        for (int k = 0; k < 4; ++k) {
          connectedToInputFrom[i][j][k] = connectedToOutputFrom[i][j][k] = false;
        }
      }
    }
    for (int i = 0; i < 8; ++i) {
      if (!inPipe[i].disabled) {
        searchQueue.enqueue(Triplet<int>(i, 0));
        connectedToInput[i][0] = true;
      }
    }
    while (!searchQueue.isEmpty()) {
      auto target = searchQueue.dequeue();
      if (target.b > 0 and target.c != 0 and
          !vPipe[target.a][target.b - 1].disabled) {
        if (!connectedToInputFrom[target.a][target.b - 1][2]) {
          connectedToInputFrom[target.a][target.b - 1][2] = true;
          connectedToInput[target.a][target.b - 1] = true;
          searchQueue.enqueue(Triplet<int>(target.a, target.b - 1, 2));
        }
      }
      if (target.a < cols - 1 and target.c != 1 and
          !hPipe[target.a][target.b].disabled) {
        if (!connectedToInputFrom[target.a + 1][target.b][3]) {
          connectedToInputFrom[target.a + 1][target.b][3] = true;
          connectedToInput[target.a + 1][target.b] = true;
          searchQueue.enqueue(Triplet<int>(target.a + 1, target.b, 3));
        }
      }
      if (target.b < rows - 1 and target.c != 2 and
          !vPipe[target.a][target.b].disabled) {
        if (!connectedToInputFrom[target.a][target.b + 1][0]) {
          connectedToInputFrom[target.a][target.b + 1][0] = true;
          connectedToInput[target.a][target.b + 1] = true;
          searchQueue.enqueue(Triplet<int>(target.a, target.b + 1, 0));
        }
      }
      if (target.a > 0 and target.c != 3 and
          !hPipe[target.a - 1][target.b].disabled) {
        if (!connectedToInputFrom[target.a - 1][target.b][1]) {
          connectedToInputFrom[target.a - 1][target.b][1] = true;
          connectedToInput[target.a - 1][target.b] = true;
          searchQueue.enqueue(Triplet<int>(target.a - 1, target.b, 1));
        }
      }
    }

    for (int i = 0; i < 8; ++i) {
      if (!outPipe[i].disabled) {
        searchQueue.enqueue(Triplet<int>(i, rows - 1, 2));
        connectedToOutput[i][rows - 1] = true;
      }
    }
    while (!searchQueue.isEmpty()) {
      auto target = searchQueue.dequeue();
      if (target.b > 0 and target.c != 0 and
          !vPipe[target.a][target.b - 1].disabled) {
        if (!connectedToOutputFrom[target.a][target.b - 1][2]) {
          connectedToOutputFrom[target.a][target.b - 1][2] = true;
          connectedToOutput[target.a][target.b - 1] = true;
          searchQueue.enqueue(Triplet<int>(target.a, target.b - 1, 2));
        }
      }
      if (target.a < cols - 1 and target.c != 1 and
          !hPipe[target.a][target.b].disabled) {
        if (!connectedToOutputFrom[target.a + 1][target.b][3]) {
          connectedToOutputFrom[target.a + 1][target.b][3] = true;
          connectedToOutput[target.a + 1][target.b] = true;
          searchQueue.enqueue(Triplet<int>(target.a + 1, target.b, 3));
        }
      }
      if (target.b < rows - 1 and target.c != 2 and
          !vPipe[target.a][target.b].disabled) {
        if (!connectedToOutputFrom[target.a][target.b + 1][0]) {
          connectedToOutputFrom[target.a][target.b + 1][0] = true;
          connectedToOutput[target.a][target.b + 1] = true;
          searchQueue.enqueue(Triplet<int>(target.a, target.b + 1, 0));
        }
      }
      if (target.a > 0 and target.c != 3 and
          !hPipe[target.a - 1][target.b].disabled) {
        if (!connectedToOutputFrom[target.a - 1][target.b][1]) {
          connectedToOutputFrom[target.a - 1][target.b][1] = true;
          connectedToOutput[target.a - 1][target.b] = true;
          searchQueue.enqueue(Triplet<int>(target.a - 1, target.b, 1));
        }
      }
    }


    for (int i = 0; i < 8; ++i) {
      if (!connectedToInput[i][rows - 1]) {
        outPipe[i].defunct = !outPipe[i].disabled;
      } else {
        outPipe[i].defunct = false;
      }
      if (!connectedToOutput[i][0]) {
        inPipe[i].defunct = !inPipe[i].disabled;
      } else {
        inPipe[i].defunct = false;
      }
      for (int j = 0; j < 7; ++j) {
        if (!connectedToInput[i][j] or
            !connectedToInput[i][j + 1] or
            !connectedToOutput[i][j] or
            !connectedToOutput[i][j + 1] or
            (!connectedToInputFrom[i][j][2] and !connectedToOutputFrom[i][j][2]) or
            (!connectedToInputFrom[i][j + 1][0] and !connectedToOutputFrom[i][j + 1][0])) {
          vPipe[i][j].defunct = !vPipe[i][j].disabled;
        } else {
          vPipe[i][j].defunct = false;
        }
        if (!connectedToInput[j][i] or
            !connectedToInput[j + 1][i] or
            !connectedToOutput[j][i] or
            !connectedToOutput[j + 1][i] or
            (!connectedToInputFrom[j][i][1] and !connectedToOutputFrom[j][i][1]) or
            (!connectedToInputFrom[j + 1][i][3] and !connectedToOutputFrom[j + 1][i][3])) {
          hPipe[j][i].defunct = !hPipe[j][i].disabled;
        } else {
          hPipe[j][i].defunct = false;
        }
      }
    }
    /*for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        std::cout << connectedToInputFrom[j][i][0] << " ";
      }
      std::cout << "   ";
      for (int j = 0; j < 8; ++j) {
        std::cout << connectedToInputFrom[j][i][1] << " ";
      }
      std::cout << "   ";
      for (int j = 0; j < 8; ++j) {
        std::cout << connectedToInputFrom[j][i][2] << " ";
      }
      std::cout << "   ";
      for (int j = 0; j < 8; ++j) {
        std::cout << connectedToInputFrom[j][i][3] << " ";
      }
      std::cout << "   ";
      std::cout << std::endl;
    }
    std::cout << std::endl;
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        std::cout << connectedToOutputFrom[j][i][0] << " ";
      }
      std::cout << "   ";
      for (int j = 0; j < 8; ++j) {
        std::cout << connectedToOutputFrom[j][i][1] << " ";
      }
      std::cout << "   ";
      for (int j = 0; j < 8; ++j) {
        std::cout << connectedToOutputFrom[j][i][2] << " ";
      }
      std::cout << "   ";
      for (int j = 0; j < 8; ++j) {
        std::cout << connectedToOutputFrom[j][i][3] << " ";
      }
      std::cout << "   ";
      std::cout << std::endl;
    }*/
  }
  calculated = false;
  emit chipChanged();
}

void Chip::initializeChip() {
  setSize(8, 8);
  for (int i = 0; i < 7; ++i) {
    vLength[i] = hLength[i] = tempVLength[i] = tempHLength[i] = 1800;
  }
  calcAccLengths();
  calcTempAccLengths();
  for (int i = 0; i < 8; ++i) {
    inPipe[i] = outPipe[i] = PipeInfo();
    inPipe[i].rate = 200;
    inPipe[i].concentration = 0;
    for (int j = 0; j < 7; ++j) {
      vPipe[i][j] = hPipe[j][i] = PipeInfo();
    }
  }
}

bool Chip::validateInputs(Pipe* target, int width, int length) {
  if (length <= 400 or length >= 4000 or width <= 0) {
    return false;
  }
  if (target->pipeType == PipeType::vertical) {
    if (target->xIndex > 0) {
      if (2 * CHIP.hLength[target->xIndex - 1] - 400 <
          CHIP.vPipe[target->xIndex - 1][target->yIndex].width + width) {
        return false;
      }
    }
    if (target->xIndex < CHIP.cols - 1) {
      if (2 * CHIP.hLength[target->xIndex] - 400 <
          CHIP.vPipe[target->xIndex + 1][target->yIndex].width + width) {
        return false;
      }
    }
  } else {
    if (target->yIndex > 0) {
      if (2 * CHIP.vLength[target->yIndex - 1] - 400 <
          CHIP.hPipe[target->xIndex][target->yIndex - 1].width + width) {
        return false;
      }
    }
    if (target->yIndex < CHIP.rows - 1) {
      if (2 * CHIP.vLength[target->yIndex] - 400 <
          CHIP.hPipe[target->xIndex][target->yIndex + 1].width + width) {
        return false;
      }
    }
  }
  return true;
}

void Chip::setPipeInfo(Pipe* target, int width, int length) {
  target->info->width = width;
  if (target->info->length != length) {
    markSizeChanging();
    switch (target->pipeType) {
      case PipeType::input: {
        for (auto& p: inPipe) {
          p.length = length;
        }
        break;
      }
      case PipeType::output: {
        for (auto& p: outPipe) {
          p.length = length;
        }
        break;
      }
      case PipeType::vertical: {
        for (int i = 0; i < 8; ++i) {
          vPipe[i][target->yIndex].length = length;
        }
        vLength[target->yIndex] = length;
        break;
      }
      case PipeType::horizontal: {
        for (int i = 0; i < 8; ++i) {
          hPipe[target->xIndex][i].length = length;
        }
        hLength[target->xIndex] = length;
        break;
      }
      default: {
        throw std::range_error("pipe type out of range");
      }
    }
    calcAccLengths();
  }
  calculated = false;
}

void Chip::markSizeChanging() {
  ++sizeChanging;
  QTimer::singleShot(600, [&]() { --sizeChanging; });
}

void Chip::setSize(int r, int c) {
  calculated = false;
  rows = r;
  cols = c;
  for (int i = 0; i < 8; ++i) {
    if (i < cols) {
      inPipe[i].visible = outPipe[i].visible = true;
    } else {
      inPipe[i].visible = outPipe[i].visible = inPipe[i].defunct = outPipe[i].defunct = false;
      inPipe[i].disabled = outPipe[i].disabled = true;
    }
    for (int j = 0; j < 7; ++j) {
      if (i < cols and j < rows - 1) {
        vPipe[i][j].visible = true;
      } else {
        vPipe[i][j].visible = vPipe[i][j].defunct = false;
        vPipe[i][j].disabled = true;
      }
      if (i < rows and j < cols - 1) {
        hPipe[j][i].visible = true;
      } else {
        hPipe[j][i].visible = hPipe[j][i].defunct = false;
        hPipe[j][i].disabled = true;
      }
    }
  }
  markSizeChanging();
}

