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
    Metrics(const std::string ipport);
    Metrics(const Metrics &) = delete;
    Metrics &operator=(const Metrics &) = delete;

    static Metrics *getInstance(const std::string initArg = std::string())
    {
        static Metrics _instance(initArg);
        return &_instance;
    }

public:
    void update_chain_height(size_t num);
    void update_peer_count(size_t num);
    void update_orphan_blocks(size_t num);
    void inc_recv_bytes(size_t num, const std::string from);
    void inc_send_bytes(size_t num, const std::string to);

private:
    void createRegistry();

    prometheus::Exposer *exposer;
    std::shared_ptr<prometheus::Registry> registry;

    prometheus::Family<prometheus::Gauge> *chain_height;
    prometheus::Family<prometheus::Gauge> *peer_gauge;
    prometheus::Family<prometheus::Gauge> *orphan_gauge;
    prometheus::Family<prometheus::Counter> *packet_counter;
};
