#pragma once

#include <array>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>
#include <thread>
#include <string>

#include <prometheus/counter.h>
#include <prometheus/exposer.h>
#include <prometheus/registry.h>

class Metrics
{
public:
    Metrics();
    Metrics(const Metrics &) = delete;
    Metrics &operator=(const Metrics &) = delete;

    static Metrics *getInstance()
    {
        static Metrics _instance;
        return &_instance;
    }

public:
    bool StartExposter();
    void update_peer_count(size_t num);
    void inc_recv_bytes(size_t num, const std::string from);
    void inc_send_bytes(size_t num, const std::string to);

private:
    prometheus::Exposer *exposer;
    std::shared_ptr<prometheus::Registry> registry;
    prometheus::Family<prometheus::Gauge> *peer_gauge;
    prometheus::Family<prometheus::Counter> *packet_counter;
};
