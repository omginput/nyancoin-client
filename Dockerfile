FROM ubuntu:20.04 as builder
LABEL maintainer="Sophie Luna Schumann <docker@luna.vg>"

# Essential packages
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends -y git ca-certificates build-essential gcc make libupnp-dev libtool autotools-dev autoconf pkg-config libssl-dev libboost-all-dev libdb5.3++-dev libdb5.3-dev libminiupnpc-dev && apt-get clean

# Run build script
RUN git clone https://github.com/Nyancoins/nyancoin-client.git && \
    cd nyancoin-client/src && \
    make -f makefile.unix STATIC=1 RELEASE=1 -j$(nproc) && \
    strip -s nyancoind && \
    cp -v nyancoind /usr/bin


FROM ubuntu:20.04
COPY --from=builder /usr/bin/nyancoind /usr/bin
# Start nyancoind
WORKDIR /root
RUN mkdir .nyancoin && \ 
    echo "rpcuser=nyancoinrpc" > .nyancoin/nyancoin.conf && \
    echo -n "rpcpassword=" >> .nyancoin/nyancoin.conf && \ 
    (tr -cd '[:alnum:]' < /dev/urandom | fold -w30 | head -n1 >> .nyancoin/nyancoin.conf) && \
    cat /root/.nyancoin/nyancoin.conf

EXPOSE 33700 33701
VOLUME [ "/root/.nyancoin" ]

ENTRYPOINT ["nyancoind", "-detachdb", "-printtoconsole"]