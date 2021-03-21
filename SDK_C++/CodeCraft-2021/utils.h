// @Author: Yu Xin

#pragma once

namespace utils {
template <typename T>
T DivideCeil(T dividend, T divisor) {
    return 1 + ((dividend - 1) / divisor);
}
} // namespace utils
