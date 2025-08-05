#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <string>
#include <cctype>

// Thread-safe queue
template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cv;
public:
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(value);
        cv.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return !queue.empty(); });
        T val = queue.front();
        queue.pop();
        return val;
    }
};

// Queues for communication
ThreadSafeQueue<std::string> queueRawText;
ThreadSafeQueue<std::string> queueCleanedText;
ThreadSafeQueue<std::string> queueAnalyzedResult;

// Thread 1: Loader
void loaderThread(const std::string& filename) {
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        std::cout << "[Loader] loaded: " << line << std::endl;
        queueRawText.push(line);
    }
    infile.close();
}

int main() {
    std::string inputFile = "imdb_sample.txt"; // <-- เตรียมไฟล์นี้ไว้ข้างนอก
    std::thread loader(loaderThread, inputFile);

    // code Thread 2-3-4 จะตามมาในขั้นถัดไป
    loader.join();

    return 0;
}