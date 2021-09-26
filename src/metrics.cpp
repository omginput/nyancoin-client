#include "metrics.h"

using namespace prometheus;

Metrics::Metrics(const std::string ipport)
{
    // create an http server running on port 8080
    if (ipport.length() <= 0) {
        this->exposer = new Exposer{"127.0.0.1:8080"};
    } else {
        this->exposer = new Exposer{ipport};
    }
    this->createRegistry();
}

void Metrics::createRegistry()
{
    // create a metrics registry
    // @note it's the users responsibility to keep the object alive
    this->registry = std::make_shared<Registry>();

    // add a new counter family to the registry (families combine values with the
    // same name, but distinct label dimensions)
    //
    // @note please follow the metric-naming best-practices:
    // https://prometheus.io/docs/practices/naming/

    this->chain_height = &BuildGauge()
                                .Name("nyancoin_blockchain_height")
                                .Help("Number of confirmed blocks in blockchain")
                                .Register(*registry);

    this->packet_counter = &BuildCounter()
                                .Name("nyancoin_packets_total")
                                .Help("Number of observed packets")
                                .Register(*registry);

    this->peer_gauge = &BuildGauge()
                            .Name("nyancoin_peers_total")
                            .Help("Number of observed peers")
                            .Register(*registry);

    this->orphan_gauge = &BuildGauge()
                            .Name("nyan_orphan_blocks")    
                            .Help("Number of observed orphaned blocks.")
                            .Register(*registry);

    this->mempool_gauge = &BuildGauge()
                            .Name("nyan_mempool_size")
                            .Help("Size of the mempool")
                            .Register(*registry);

    this->exposer->RegisterCollectable(registry);
}

void Metrics::update_chain_height(size_t num)
{
    this->chain_height->Add({}).Set((double)num);
}

void Metrics::update_peer_count(size_t num)
{
    this->peer_gauge->Add({}).Set((double)num);
}

void Metrics::update_orphan_blocks(size_t num)
{
    this->orphan_gauge->Add({}).Set((double)num);
}

void Metrics::update_mempool(size_t num)
{
    this->mempool_gauge->Add({}).Set((double)num);
}

void Metrics::inc_recv_bytes(size_t num, const std::string from)
{
    this->packet_counter->Add({{"protocol", "tcp"}, {"direction", "rx"}, {"address", from.c_str()}}).Increment((double)num);
}

void Metrics::inc_send_bytes(size_t num, const std::string to)
{
    this->packet_counter->Add({{"protocol", "tcp"}, {"direction", "tx"}, {"address", to.c_str()}}).Increment((double)num);
}
