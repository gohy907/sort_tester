#include <ctime>
#include <fstream>
#include <vector>

namespace tester {

struct Config {
        void (*const sort)(std::vector<int> &);
        const unsigned long long number_of_tests;
        const size_t length;
        const int min;
        const int max;
        const bool critical;

    public:
        Config(void (*const sort)(std::vector<int> &),
               const unsigned long long number_of_tests, const size_t length,
               const int min, const int max, const bool critical)
            : sort(sort),
              number_of_tests(number_of_tests),
              length(length),
              min(min),
              max(max),
              critical(critical) {}
        Config(void (*const sort)(std::vector<int> &),
               const unsigned long long number_of_tests, const size_t length,
               const int min, const int max)
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
        void test(const std::vector<Config> &configs);
        void benchmark(const std::vector<BenchmarkConfig> &benchmark_configs);
        void start_once(void (*const sort)(std::vector<int> &),
                        std::vector<int> &numbers);
        std::vector<std::clock_t>
        average_times(const std::vector<Config> &test_configs);
        Tester() {}

    private:
        int randFromRange(const int start, const int end);
        void out_vector(std::ofstream &stream, const std::vector<int> &numbers);
        std::clock_t evaluate_time(void (*const sort)(std::vector<int> &),
                                   std::vector<int> &numbers);
        void validate(const std::vector<int> &initial_numbers,
                      const std::vector<int> &numbers);
        void test_once(const Config &config);

        std::clock_t average_time(const Config &config);
        std::vector<int> construct_vector(const Config &config);
};

} // namespace tester
