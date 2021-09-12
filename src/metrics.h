#pragma once

#include <array>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>
#include <thread>

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

private:
    prometheus::Exposer *exposer;
    std::shared_ptr<prometheus::Registry> registry;
    prometheus::Family<prometheus::Gauge> *peer_gauge;
    prometheus::Family<prometheus::Counter> *packet_counter;
    prometheus::Counter *tcp_rx_counter;
    prometheus::Counter *tcp_tx_counter;
};
