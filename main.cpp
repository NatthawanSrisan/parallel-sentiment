#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include "queue.h"
#include "utils.h"

// Define thread-safe queues
ThreadSafeQueue<std::string> queueRawText;
ThreadSafeQueue<std::string> queueCleanedText;
ThreadSafeQueue<std::string> queueAnalyzedResult;

int main() {
    std::vector<std::string> fakeData;

    // Load data from test.txt
    std::ifstream inFile("test.txt");
    std::string line;
    while (std::getline(inFile, line)) {
        if (!line.empty()) {
            fakeData.push_back(line);
        }
    }
    inFile.close();

    if (fakeData.empty()) {
        std::cerr << "No data found in test.txt. Exiting." << std::endl;
        return 1;
    }

    // Thread 1: Loader
    std::thread loader([&]() {
        for (const auto& text : fakeData) {
            std::cout << "[Loader] pushed: " << text << std::endl;
            queueRawText.push(text);
        }
    });

    // Thread 2: Preprocessor
    std::thread preprocessor([&]() {
        for (size_t i = 0; i < fakeData.size(); ++i) {
            std::string raw = queueRawText.pop();
            std::string cleaned = cleanText(raw);
            std::cout << "[Preprocessor] cleaned: " << cleaned << std::endl;
            queueCleanedText.push(cleaned);
        }
    });

    // Thread 3: Analyzer
    std::thread analyzer([&]() {
        for (size_t i = 0; i < fakeData.size(); ++i) {
            std::string cleaned = queueCleanedText.pop();
            std::string sentiment = analyzeSentiment(cleaned);
            std::cout << "[Analyzer] " << cleaned << " | Sentiment: " << sentiment << std::endl;

            std::string result = cleaned + "|" + sentiment;
            queueAnalyzedResult.push(result);
        }
    });

    // Thread 4: Saver
    std::thread saver([&]() {
        std::ofstream outFile("result.txt");
        for (size_t i = 0; i < fakeData.size(); ++i) {
            std::string combined = queueAnalyzedResult.pop();
            size_t delimiter = combined.find("|");
            std::string cleaned = combined.substr(0, delimiter);
            std::string sentiment = combined.substr(delimiter + 1);

            std::cout << "[Saver] wrote: " << cleaned << " | Sentiment: " << sentiment << std::endl;
            outFile << cleaned << " | Sentiment: " << sentiment << "\n";
        }
        outFile.close();
    });

    // Join all threads
    loader.join();
    preprocessor.join();
    analyzer.join();
    saver.join();

    return 0;
}