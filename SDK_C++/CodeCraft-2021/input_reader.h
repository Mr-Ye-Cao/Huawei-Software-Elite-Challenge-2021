// @Author: Yu Xin

#pragma once

#include <string>

namespace {
const char kSpace[] = " ";
} // namespace

class InputReader {
  public:
    InputReader() = default;
    ~InputReader() = default;
    void ReadInputFile(std::string file_location);
    void ReadInputCin();

  private:


    
};
