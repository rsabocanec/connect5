#include <random>
#include <span>
#include <array>
#include <algorithm>
#include <iostream>

#include <cmath>

#include <termios.h>
#include <unistd.h>

namespace {
int get_character() {
    struct termios oldattr, newattr;

    tcgetattr(STDIN_FILENO, &oldattr);

    newattr = oldattr;
    newattr.c_lflag &= ~ICANON;
    newattr.c_lflag &= ~ECHO;
    newattr.c_cc[VMIN] = 1;
    newattr.c_cc[VTIME] = 0;
    ::tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    
    auto ch = ::getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return ch; 
}

constexpr std::size_t values_count{6ull};

template <typename T>
void print_values(std::span<T> values) {
    std::cout << '\n';

    std::size_t index{};
    for (auto it = std::cbegin(values); it != std::cend(values); ++it) {

        if (index == 0 
        ||  (index == values.size() - 1 && index % 2 != 0)) {
            std::cout << '\t';
        }

        std::cout << '\t' << *it;

        if (index++ %2 == 0) {
            std::cout << '\n';
            if (index > 1 && index < values.size() - 1) {
                std::cout << '\n';
            }
        }
        else {
            std::cout << '\t';
        }
    }

    std::cout << '\n';
}
}

int main() {
    std::random_device device;
    std::uniform_int_distribution<int> dist(0, 9);

    std::vector<int> values_list{};
    for (auto i = 0ull; i < values_count; ++i) {
        values_list.push_back(dist(device));
    }

    print_values(std::span<int>(values_list));

    std::cout << "\n\nEnter solution:\n";

    int total{};
    int old_value{-1};

    while (!values_list.empty()) {
        auto value = get_character() - static_cast<int>('0');

        auto end_pos = std::cend(values_list);
        auto pos = std::find(std::cbegin(values_list), end_pos, value);

        if (pos != end_pos) {
            std::cout << value;

            if (old_value != -1) {
                if (value == 0 && old_value > 5) {
                    value = 10;
                }

                if (value == old_value) {
                    total += 10;
                }
                else {
                    total += ::abs(value - old_value) - 1;
                }

                std::cout << "\nTotal distance is: " << total << '\n';
            }

            old_value = value;

            values_list.erase(pos);
            print_values(std::span<int>(values_list));
        }
    }

    std::cout << "\nTotal distance is: " << total << '\n';

    return 0;
}