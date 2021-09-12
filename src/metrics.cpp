#include "metrics.h"

Metrics::Metrics()
{
    using namespace prometheus;

    // create an http server running on port 8080
    this->exposer = new Exposer{"127.0.0.1:8080"};

    // create a metrics registry
    // @note it's the users responsibility to keep the object alive
    this->registry = std::make_shared<Registry>();

    // add a new counter family to the registry (families combine values with the
    // same name, but distinct label dimensions)
    //
    // @note please follow the metric-naming best-practices:
    // https://prometheus.io/docs/practices/naming/

    this->packet_counter = &BuildCounter()
                                .Name("nyancoin_packets_total")
                                .Help("Number of observed packets")
                                .Register(*registry);

    this->peer_gauge = &BuildGauge()
                            .Name("nyancoin_peers_total")
                            .Help("Number of observed peers")
                            .Register(*registry);

    // add and remember dimensional data, incrementing those is very cheap
    this->tcp_rx_counter =
        &packet_counter->Add({{"protocol", "tcp"}, {"direction", "rx"}});
    this->tcp_tx_counter =
        &packet_counter->Add({{"protocol", "tcp"}, {"direction", "tx"}});

    this->exposer->RegisterCollectable(registry);
}

void Metrics::update_peer_count(size_t num)
{
    this->peer_gauge->Add({}).Set((double)num);
}
