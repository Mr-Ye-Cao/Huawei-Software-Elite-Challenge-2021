// @Author: Yu Xin

#pragma once

#include <string>

namespace {
const char kSpace[] = " ";
} // namespace

class InputReader {
  public:
    InputReader(std::string file_location);
    ~InputReader() = default;
    void ReadInputFile();
    void ReadInputCin();

  private:
    const std::string file_location_;
    // Add data structures for holding these information

    
};
