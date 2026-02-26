# What is this?

This library allows to easily create various tests and benchmarks for your sorting algorithms in C++.

# Basic testing

You can use `std::vector<Config>` to describe configs for testing your function in various scenarios.

`Config` settings look like this:

```cpp
struct Config {
        void (*const sort)(std::vector<int> &);
        const unsigned long long number_of_tests;
        const size_t length;
        const int min;
        const int max;
        const bool critical;
}
```

- `sort` is your function to test;
- `number_of_tests` is, well, number of tests that will be executed with this config;
- `length` is size of allocated `std::vector` to test;
- `min` and `max` are constraints for numbers that will fill up `std::vector` to test.

Basically, every iteration new `std::vector` will be allocated and filled up with random integers ranging from `min` to `max`. 

- `critical` is flag which you want to set `true` to add `INT_MIN` and `INT_MAX` to the `std::vector` in test. Note that `size()` of created `std::vector` will exceed `length` by 2.

And then you can start testing with `test()` with `std::vector<Config>`, which runs your function with randomly constructed `std::vector`, computes time in which your function have completed sorting such vector and checks it for correctness in various passed configurations. 

For example:

```cpp
#include "tester.hpp"
#include <vector>

void insertion_sort(std::vector<int> &vec) {
    int left = 0;
    int right = vec.size();
    for (int i = left; i < right; ++i) {
        for (int j = i - 1; j >= left; --j) {
            if (vec[j] > vec[j + 1]) {
                int tmp = vec[j + 1];
                vec[j + 1] = vec[j];
                vec[j] = tmp;
            } else {
                break;
            }
        }
    }
}

int main() {
    tester::Tester t;
    t.test({tester::Config(insertion_sort, 10, 50, -1'000, 1'000)});
}
```

If your function failes to sort vector properly, `abort()` will be executed and you will have input and output vector in "error.txt" next to your main `.cpp` file, so you can run your algorithm with this data and figure out what have gone wrong.

_TODO: Make this debug data JSON-compatible._

While your function is being tested, CPU time in which it sorted vector will be output to "benchmark.txt".

## Debugging

You can use `start_once()` method with exact `std::vector<int>` that failed the test to debug algorithm.

# Benchmarking

You can properly benchmark your algorithm by calling `benchmark()` method of `Tester` with `std::vector<BenchmarkConfig>`.

`BenchmarkConfig` looks like this:

```cpp
struct BenchmarkConfig {
        void (*const sort)(std::vector<int> &);
        const size_t start_size;
        const size_t size_to_iterate;
        const int min;
        const int max;
        const bool critical;
}
```

- `start_size` is initial length of vector at the start of benchmarks;
- `size_to_iterate` is offset by which `start_size` is incremented every iteration of benchmark.

Other fields are same as in `Config`.

Example of using `benchmark()`:

```cpp
tester::Tester t;
t.benchmark({tester::BenchmarkConfig(insertion_sort, 100'000, 50'000, -1'000, 1'000)});
```

Results of benchmark will iteratively write to "benchmark.txt", same as in `test()` method.

# Computing average time

You can also measure average time it took your function to sort data in specific configuration.

It can be computed in `average_times()` method with `std::vector<Config>` to pass. 
This method will compute durations of time in which your function managed to sort data, average it by `number_of_tests` and output `std::vector<std::clock_t>` where every `std::clock_t` corresponds to each `Config` respectively by indices.

Example of using `average_times()`:

```cpp
tester::Tester t;
std::vector<std::clock_t> times = t.average_times(
    {tester::Config(insertion_sort, 10, 5000, -1'000, 1'000)});
for (size_t i = 0; i < times.size(); ++i) {
    std::cout << times[i] << " ";
}
```

# Notes

- Data output to "benchmark.txt" is Desmos-compatible, you can copy strings of file and paste them into Desmos to visualize and estimate time complexity of your algorithm.
- In current realization validation of algorithm is being done in `benchmark()` and `average_times()`  although these methods do not have to do that directly. It have benefits such as notification of developer if something goes wrong in theirs algorithm by `abort()` but necessity of this functionality is still up to debate. 

