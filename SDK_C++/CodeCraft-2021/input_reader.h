// @Author: Yu Xin

#pragma once

#include <string>

namespace {
const char kSpace[] = " ";
const char kComma[] = ", ";
const char kZero[] = "0";
const char kAdd = 'a';
} // namespace

class InputReader {
  public:
    InputReader(std::string file_location);
    ~InputReader() = default;
    void ReadInputFile();
    void ReadInputCin();

  private:
    const std::string file_location_;
    // TODO(Yu Xin): Add data structures for holding these information

    
};
