#include <ctime>
#include <fstream>
#include <vector>

namespace tester {

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
} // namespace tester
