#pragma once 

#include <array>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>
#include <thread>

class Metrics {
    public:
    
    static Metrics* getInstance() {
        if (_instance == nullptr) {
            _instance = new Metrics();
        }
        return _instance;
    }

    private:
    static Metrics* _instance;
};
