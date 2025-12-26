#include <random>
#include <span>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
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

struct compare_vector {
    static void add_total(int value, int &old_value, int &total) {
        if (value == old_value) {
            total += 9;
        }
        else {
            if (old_value != -1) {
                if (old_value < value) {
                    total += std::min(value - old_value, old_value + 10 - value) - 1;
                }
                else {
                    total += std::min(old_value - value, value + 10 - old_value) - 1;
                }
            }
        }

        old_value = value;
    }

    static int calculate(const std::vector<int> &s) {
        int total{};
        int old_value{-1};

        for (auto value : s) {
            add_total(value, old_value, total);
        }
        
        return total;
    }

    bool operator()(const std::vector<int> &lhs, const std::vector<int> &rhs) const {
        return calculate(lhs) < calculate(rhs);
    }
};

using optimum_list = std::multiset<std::vector<int>, compare_vector>;

optimum_list populate(std::vector<int> input) {
    optimum_list result{};

    do
    {
        result.insert(input);
        for (auto i : input) {
            std::cout << i << ' ';
        }

        std::cout << "==> " << compare_vector::calculate(input) << '\n';
    }
    while (std::ranges::next_permutation(input.begin(), input.end()).found);

    return result;
}
}

int main() {
    std::random_device device;
    std::uniform_int_distribution<int> dist(0, 9);

    std::vector<int> values_list{};
    for (auto i = 0ull; i < values_count; ++i) {
        values_list.push_back(dist(device));
    }

    auto ol = populate(values_list);

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

            compare_vector::add_total(value, old_value, total);

            values_list.erase(pos);
            print_values(std::span<int>(values_list));
        }
    }

    std::cout << "\nTotal distance is: " << total << '\n';

    if (!ol.empty()) {
        auto it = std::cbegin(ol);
        auto it_end = std::cend(ol);// ol.upper_bound(*it);

        std::cout << "\nOptimal solution is one of:\n" << '\n';
        for (; it != it_end; ++it) {
            // Print the span
            for (auto value : *it) {
                std::cout << value << ' ';
            }

            std::cout << "==> " << compare_vector::calculate(*it) << '\n';
        }
    }

    return 0;
}