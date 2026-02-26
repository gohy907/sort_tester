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
        const size_t number_of_tests;
        const size_t length;
        const int min;
        const int max;
        const bool critical;

    public:
        Config(void (*const sort)(std::vector<int> &),
               const size_t number_of_tests, const size_t length, const int min,
               const int max, const bool critical)
            : sort(sort),
              number_of_tests(number_of_tests),
              length(length),
              min(min),
              max(max),
              critical(critical) {}
        Config(void (*const sort)(std::vector<int> &),
               const size_t number_of_tests, const size_t length, const int min,
               const int max)
            : Config(sort, number_of_tests, length, min, max, false) {}
};

struct BenchmarkConfig {
        void (*const sort)(std::vector<int> &);
        const size_t start_size;
        const size_t size_to_iterate;
        const int min;
        const int max;
        const bool critical;

    public:
        BenchmarkConfig(void (*const sort)(std::vector<int> &),
                        const size_t start_size, const size_t size_to_iterate,
                        const int min, const int max, const bool critical)
            : sort(sort),
              start_size(start_size),
              size_to_iterate(size_to_iterate),
              min(min),
              max(max),
              critical(critical) {}
        BenchmarkConfig(void (*const sort)(std::vector<int> &),
                        const size_t start_size, const size_t size_to_iterate,
                        const int min, const int max)
            : BenchmarkConfig(sort, start_size, size_to_iterate, min, max,
                              false) {}
};

class Tester {
    public:
        void start();
        void benchmark(const BenchmarkConfig &config);
        void start_once(void (*const sort)(std::vector<int> &),
                        std::vector<int> &numbers);
        std::vector<std::clock_t> average_times();
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

        std::clock_t average_time(const Config &config);
        std::vector<int> construct_vector(const Config &config);
        const std::vector<Config> test_configs;
};

void Tester::benchmark(const BenchmarkConfig &config) {
    const size_t max_length = 100000000;
    for (size_t i = config.start_size; i < max_length;
         i += config.size_to_iterate) {
        Config new_config(config.sort, 1, i, config.min, config.max,
                          config.critical);

        test(new_config);
    }
}

std::vector<std::clock_t> Tester::average_times() {
    std::vector<std::clock_t> times(test_configs.size(), 0);
    for (size_t i = 0; i < test_configs.size(); ++i) {
        times[i] = average_time(test_configs[i]);
    }
    return times;
};

std::clock_t Tester::average_time(const Config &config) {
    std::clock_t total_time = 0;
    for (size_t i = 0; i < config.number_of_tests; ++i) {
        std::vector<int> numbers = construct_vector(config);
        std::vector<int> initial_numbers = numbers;
        std::clock_t time = evaluate_time(config.sort, numbers);
        validate(initial_numbers, numbers);
        total_time += static_cast<long double>(time) / config.number_of_tests;
    }
    return static_cast<std::clock_t>(total_time);
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

std::vector<int> Tester::construct_vector(const Config &config) {
    std::vector<int> numbers(config.length, 0);
    for (size_t j = 0; j < config.length; ++j) {
        numbers[j] = randFromRange(config.min, config.max);
    }
    if (config.critical) {
        numbers.push_back(INT_MAX);
        numbers.push_back(INT_MIN);
    }
    return numbers;
}

void Tester::test(const Config &config) {
    for (size_t i = 0; i < config.number_of_tests; ++i) {
        std::vector<int> numbers = construct_vector(config);
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
