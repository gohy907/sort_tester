#include <climits>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

std::mt19937 rnd(time(NULL));
namespace tester {

std::ofstream out("benchmark.txt");
std::ofstream error("error.txt");

struct Config {
        void (*const sort)(std::vector<int> &);
        const int number_of_tests;
        const size_t length;
        const int min;
        const int max;
        const bool critical;

    public:
        Config(void (*const sort)(std::vector<int> &),
               const int number_of_tests, const size_t length, const int min,
               const int max, const bool critical)
            : sort(sort),
              number_of_tests(number_of_tests),
              length(length),
              min(min),
              max(max),
              critical(critical) {}
        Config(void (*const sort)(std::vector<int> &),
               const int number_of_tests, const size_t length, const int min,
               const int max)
            : Config(sort, number_of_tests, length, min, max, false) {}
};

class Tester {
    public:
        void start();
        void benchmark(const Config &config);
        void start_once(void (*const sort)(std::vector<int> &),
                        std::vector<int> &numbers);
        Tester() {}
        Tester(const std::vector<Config> &test_configs)
            : test_configs(test_configs) {}

    private:
        int randFromRange(const int start, const int end);
        void out_vector(std::ofstream &stream, const std::vector<int> &numbers);
        std::clock_t evaluate_time(void (*const sort)(std::vector<int> &),
                                   std::vector<int> &numbers);
        void validate(const std::vector<int> &initial_numbers,
                      const std::vector<int> &numbers);
        void test(const Config &config);
        const std::vector<Config> test_configs;
};

void Tester::benchmark(const Config &config) {
    const size_t max_length = 100000000;
    for (size_t i = 2; i < max_length; ++i) {
        Config new_config(config.sort, 1, i, config.min, config.max,
                          config.critical);
        test(new_config);
        if (i % 1000000 == 0) {
            std::cout << i << " ITERATIONS COMPLETE OUT OF " << max_length
                      << std::endl;
        }
    }
}

void Tester::start_once(void (*const sort)(std::vector<int> &),
                        std::vector<int> &numbers) {
    std::vector<int> initial_numbers = numbers;
    std::clock_t time = evaluate_time(sort, numbers);
    validate(initial_numbers, numbers);

    out << "A = (" << numbers.size() << ", " << time << ")" << std::endl;
}

int Tester::randFromRange(const int start, const int end) {
    if (start == INT_MIN && end == INT_MAX) {
        return rnd();
    }
    return rnd() % (end - start + 1) + start;
}

void Tester::out_vector(std::ofstream &stream,
                        const std::vector<int> &numbers) {
    stream << "{";
    for (size_t i = 0; i < numbers.size() - 1; ++i) {
        stream << numbers[i] << ", ";
    }
    stream << numbers[numbers.size() - 1];

    stream << "}" << std::endl;
}

std::clock_t Tester::evaluate_time(void (*const sort)(std::vector<int> &),
                                   std::vector<int> &numbers) {

    std::clock_t c_start = std::clock();
    sort(numbers);
    std::clock_t c_end = std::clock();
    return c_end - c_start;
}

void Tester::validate(const std::vector<int> &initial_numbers,
                      const std::vector<int> &numbers) {
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

void Tester::test(const Config &config) {
    for (int i = 0; i < config.number_of_tests; ++i) {
        std::vector<int> numbers(config.length, 0);
        for (size_t j = 0; j < config.length; ++j) {
            numbers[j] = randFromRange(config.min, config.max);
        }
        if (config.critical) {
            numbers.push_back(INT_MAX);
            numbers.push_back(INT_MIN);
        }

        std::vector<int> initial_numbers = numbers;
        std::clock_t time = evaluate_time(config.sort, numbers);
        validate(initial_numbers, numbers);

        out << "A = (" << numbers.size() << ", " << time << ")" << std::endl;
    }
}

void Tester::start() {
    for (size_t i = 0; i < test_configs.size(); ++i) {
        const Config &config = test_configs[i];
        test(config);
    }
}
} // namespace tester
