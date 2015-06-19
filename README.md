NyanCoin (NYAN, Ɲ)
===========

<a href="http://imgur.com/Jm2KFOD"><img src="http://i.imgur.com/Jm2KFOD.png" title="Hosted by imgur.com" /></a>

- POW Scrypt Cryptocurrency
- 337 000 000 Coins
- 337 coins per block
- 1 Minute Blocks
- Subsidy halves in 500k blocks
- 3 hour retarget 
- RPC port 33700
- P2P port 33701

## Compiling ##

### NyanCoind for Linux: ###

```
cd src
make -f makefile.unix clean all STATIC=1
strip -s nyancoind
```
