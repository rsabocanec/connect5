#include <random>
#include <span>
#include <array>
#include <iostream>

namespace {
constexpr std::size_t values_count{6ull};

template <typename T>
void print_values(std::span<T> values) {
    std::size_t index{};
    for (auto it = std::cbegin(values); it != std::cend(values); ++it) {

        if (index == 0 || index == values.size() - 1) {
            std::cout << '\t';
        }

        std::cout << '\t' << *it;

        if (index++ %2 == 0) {
            std::cout << '\n';    
        }
        else {
            std::cout << '\t';
        }
    }
}
}

int main() {
    std::random_device device;
    std::uniform_int_distribution<int> dist(0, 9);

    std::array<int, values_count> values{};
    for (auto i = 0ull; i < values_count; ++i) {
        values.at(i) = dist(device);
    }

    print_values(std::span<int>(values));

    return 0;
}