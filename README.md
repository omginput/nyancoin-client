NyanCoin (NYAN, Ɲ)
===========

![Nyancoin logo](http://i.imgur.com/Jm2KFOD.png)

- POW Scrypt Cryptocurrency
- 337 000 000 Coins
- 337 coins per block
- 1 Minute Blocks
- Subsidy halves in 500k blocks
- 3 hour retarget
- RPC port 33700
- P2P port 33701

## Compiling ##

### NyanCoin daemon for Linux ###
Everything you need to get started:

Required packages:

- Boost 1.76 or later
- miniupnpc
- OpenSSL 1.1.x or 3.x.x

```sh
cd src
make -f makefile.unix clean all STATIC=1
strip -s nyancoind
```

You will need to set an rpc password, just follow the instructions `nyancoind` gives you.

### NyanCoin Wallet Qt5 ###
The full GUI wallet.

Required packages:

- Boost 1.76 or later
- Qt 5.x.x
- miniupnpc
- OpenSSL 1.1.x or 3.x.x

```sh
qmake
make -f Makefile.Release -j$(nproc)
```

--------------------------------------------------
### Docker ###
Prebuilt images for the nyancoin daemon for *x86_64* and *aarch64* are available on [Docker Hub.](https://hub.docker.com/repository/docker/nyancoin/daemon)

Otherwise:

```sh
docker build -t nyancoind .
```

For use with docker-compose:

```yaml
version: '3'
services:
    nyancoin:
        image: nyancoin/daemon:latest
        container_name: nyancoin
        restart: unless-stopped
        volumes:
            - ./data:/root/.nyancoin
        ports:
            - 127.0.0.1:33700:33700
            - 33701:33701
```


### Metrics branch ###

The metrics branch includes a prometheus exporter which can be enable by using this `nyancoind` argument: `-metrics=0.0.0.0:8080`  
or by using the metrics tag with docker images:

```yaml
version: '3'
services:
    nyancoin:
        build: 'upstream'
        image: nyancoin/daemon:metrics
        container_name: nyancoin
        restart: unless-stopped
        volumes:
            - ./data:/root/.nyancoin
        ports:
            - 127.0.0.1:8080:8080
            - 127.0.0.1:33700:33700
            - 33701:33701
```

This branch has additional external library requirements:

- prometheus-cpp
- civetweb
