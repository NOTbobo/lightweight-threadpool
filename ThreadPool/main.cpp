#include <iostream>
#include "ThreadPool.h"

int main() {
    ThreadPool pool(4);
    std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

    auto result = pool.enqueue([](int a, int b) {
        std::cout << "Worker thread id: " << std::this_thread::get_id() << std::endl;
        return a + b;
    }, 1, 2);

    std::cout << "Result: " << result.get() << std::endl;

    return 0;
}