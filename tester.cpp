#include <climits>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

namespace tester {

std::ofstream out("benchmark.txt");
std::ofstream error("error.txt");

struct Config {
        void (*sort)(std::vector<int> &);
        int number_of_tests;
        size_t length;
        int min;
        int max;
        bool critical;

    public:
        Config(void (*sort)(std::vector<int> &), int number_of_tests,
               size_t length, int min, int max, bool critical)
            : sort(sort),
              number_of_tests(number_of_tests),
              length(length),
              min(min),
              max(max),
              critical(critical) {}
        Config(void (*sort)(std::vector<int> &), int number_of_tests,
               size_t length, int min, int max)
            : Config(sort, number_of_tests, length, min, max, false) {}
};

class Tester {
    public:
        void start();
        void initialize_tests(std::vector<Config> &configs);

    private:
        int randFromRange(int start, int end);
        void out_vector(std::ofstream &stream, std::vector<int> &numbers);
        std::clock_t benchmark(void (*sort)(std::vector<int> &),
                               std::vector<int> &numbers);
        void validate(std::vector<int> &initial_numbers,
                      std::vector<int> &numbers);
        void test(Config &config);
        std::vector<Config> test_configs;
};

void Tester::initialize_tests(std::vector<Config> &configs) {
    test_configs = configs;
}

int Tester::randFromRange(int start, int end) {
    if (start == INT_MIN && end == INT_MAX) {
        return rand();
    }
    return rand() % (end - start + 1) + start;
}

void Tester::out_vector(std::ofstream &stream, std::vector<int> &numbers) {
    stream << "{";
    for (size_t i = 0; i < numbers.size(); ++i) {
        stream << numbers[i] << ", ";
    }
    stream << "}" << std::endl;
}

std::clock_t Tester::benchmark(void (*sort)(std::vector<int> &),
                               std::vector<int> &numbers) {

    std::clock_t c_start = std::clock();
    sort(numbers);
    std::clock_t c_end = std::clock();
    return c_end - c_start;
}

void Tester::validate(std::vector<int> &initial_numbers,
                      std::vector<int> &numbers) {
    for (size_t j = 1; j < numbers.size(); ++j) {
        if (numbers[j] < numbers[j - 1]) {
            error << "INPUT_DATA = ";
            out_vector(error, initial_numbers);

            error << "OUTPUT_DATA = ";
            out_vector(error, numbers);

            std::cout << "TEST FAILED. INPUT AND OUTPUT DATA ARE IN ERROR.TXT"
                      << std::endl;
            std::abort();
        }
    }
}

void Tester::test(Config &config) {
    void (*sort)(std::vector<int> &) = config.sort;
    int number_of_tests = config.number_of_tests;
    size_t &length = config.length;
    int min = config.min;
    int max = config.max;
    bool critical = config.critical;

    for (int i = 0; i < number_of_tests; ++i) {
        std::vector<int> numbers(length, 0);
        for (size_t j = 0; j < length; ++j) {
            numbers[j] = randFromRange(min, max);
        }
        if (critical) {
            numbers.push_back(INT_MAX);
            numbers.push_back(INT_MIN);
        }

        std::vector<int> initial_numbers = numbers;
        std::clock_t time = benchmark(sort, numbers);
        validate(initial_numbers, numbers);

        out << "A = (" << i << ", " << time << ")" << std::endl;
    }
}

void Tester::start() {
    for (size_t i = 0; i < test_configs.size(); ++i) {
        Config &config = test_configs[i];
        test(config);
    }
}
} // namespace tester
