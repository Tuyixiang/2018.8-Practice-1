//
// Created by Yixiang Tu on 01/09/2018.
//

#ifndef CHIP_SIMULATOR_ENUMTYPES_H
#define CHIP_SIMULATOR_ENUMTYPES_H

enum class Tools {
  select,
  add,
  remove
};

enum class PipeType {
  null,
  input,
  output,
  vertical,
  horizontal
};

enum ItemType {
  pipe = 65537,
  pipeHighlight = 65538
};

#endif //CHIP_SIMULATOR_ENUMTYPES_H
