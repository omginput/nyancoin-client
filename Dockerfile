FROM ubuntu:20.04 as builder
LABEL maintainer="Sophie Luna Schumann <docker@luna.vg>"

# Essential packages
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends -y cmake git ca-certificates build-essential gcc make libupnp-dev libtool autotools-dev autoconf pkg-config libssl-dev libboost-all-dev libdb5.3++-dev libdb5.3-dev libminiupnpc-dev && apt-get clean

WORKDIR /app
COPY . .

RUN git clone https://github.com/civetweb/civetweb.git
RUN cd civetweb && \
    mkdir _build; cd _build && \
    cmake .. -DCIVETWEB_INSTALL_EXECUTABLE=OFF -DCIVETWEB_ENABLE_SERVER_EXECUTABLE=OFF -DCIVETWEB_BUILD_TESTING=OFF -DCIVETWEB_ENABLE_CXX=ON -DCIVETWEB_ENABLE_IPV6=ON -DCIVETWEB_DISABLE_CGI=ON -DCIVETWEB_ENABLE_ZLIB=ON -DCIVETWEB_ENABLE_SSL=ON  && \
    make -j$(nproc) && make install 

WORKDIR /app
# Run build script
RUN cd src && \
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
