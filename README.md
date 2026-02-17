This library allows to easily create various tests and benchmarks for your sorting algorithms.

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
    tester::Tester t({tester::Config(insertion_sort, 10, 50, -1000, 1000)});
    t.start();
}
```

You can initialize `Tester` with `std::vector<Config>`, which describe configs for testing your function in various scenarios.

`config` settings look like this:
```cpp
struct Config {
        void (*const sort)(std::vector<int> &);
        const int number_of_tests;
        const size_t length;
        const int min;
        const int max;
        const bool critical;
}
```

- `sort` is your function to test, 
- `number_of_tests` is, well, number of tests that will be executed with this config
- `length` is size of allocated `std::vector` to test
- `min` and `max` are constraints for numbers that will fill up `std::vector` to test

Basically, every iteration new `std::vector` will be allocated and filled up with random integers ranging from `min` to `max`. 

- `critical` is flag which you want to set `true` to add `INT_MIN` and `INT_MAX` to the `std::vector` in test. Note that `size()` of created `std::vector` will exceed `length` by 2.

And then you can start testing with `start()` method, which runs your function with randomly constructed `std::vector`, computes time in which your function have completed sorting such vector and checks it for correctness. 

If your function failes to sort vector properly, `abort()` will be executed and you will have input and output vector in "error.txt" next to your main `.cpp` file, so you can run your algorithm with this data and figure out what have gone wrong.

_TODO: Make this debug data JSON-compatible._

While your function is being tested, CPU time in which it sorted vector will be output to "benchmark.txt".

You can properly benchmark your algorithm by calling `benchmark(const Config& config)` method of `Tester`. It will still use settings of config, except for `number_of_tests` and `length`. 

Data output to "benchmark.txt" is Desmos-compatible, you can copy strings of file and paste them into Desmos to visualize and estimate time complexity of your algorithm.

