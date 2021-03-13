// @Author: Yu Xin

#include <string>

namespace {
const char kSpace[] = " ";
} // namespace

class InputReader {
  public:
    InputReader();
    ~InputReader() = default;

  private:
    void ReadInputFile(std::string file_location);
    void ReadInputCin();


    
};
