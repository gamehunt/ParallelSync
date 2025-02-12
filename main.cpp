#include <iostream>
#include <mutex>
#include <thread>

#define PERF_START(id) \
    auto start_##id = std::chrono::high_resolution_clock::now();

#define PERF_END(id) \
    auto end_##id = std::chrono::high_resolution_clock::now();

#define PERF_RESULT(id) \
    std::chrono::duration_cast<std::chrono::microseconds>(end_##id - start_##id).count()

int counter = 0;
std::mutex lock;

void increase(bool should_lock) {
    for(int i = 0; i < 10000; i++) {
        if(should_lock) {
            lock.lock();
        }
        counter++;
        if(should_lock) {
            lock.unlock();
        }
    }
}

void decrease(bool should_lock) {
    for(int i = 0; i < 10000; i++) {
        if(should_lock) {
            lock.lock();
        }
        counter--;
        if(should_lock) {
            lock.unlock();
        }
    }
}

int main() {
    std::thread inc(increase, false);
    std::thread dec(decrease, false);
    
    inc.join();
    dec.join();

    std::cout << "NO LOCK: " << counter << std::endl;

    counter = 0;

    std::thread inc_lock(increase, true);
    std::thread dec_lock(decrease, true);

    inc_lock.join();
    dec_lock.join();

    std::cout << "LOCK: " << counter << std::endl;
    
    return 0;
}
