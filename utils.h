//untils.h
#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <string>
#include <mutex>

// type: pair of (label, review)
extern std::queue<std::pair<int, std::string>> reviewQueue;
extern std::mutex queueMutex;

#endif