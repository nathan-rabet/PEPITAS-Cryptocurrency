![Pepitas image](https://user-images.githubusercontent.com/13369175/121909610-60bb2800-cd2e-11eb-9510-6aef81d91b16.png)
# PEPITAS, a C cryptocurrency
PEPITAS is an EPITA project made in the last semester of the preparatory cycle.
This cryptocurrency is based on the *proof of stake*, the new validation concensus (used in Etherum 2.0).

With PEPITAS, you can test whatever a modern proof of stake based money can do :
- Send money
- Receive money
- Invest on the stake
- Validate transactions
- Earn transactions fees

All of these features are obviously secured by a 2048-bits RSA based protocol.

# Requirements
- A Linux system (Ubuntu, Arch,...)
- GNU Make
- OpenSSL
- GTK 

# Installation
1. Download the last version of the project : [PEPITAS-Cryptocurrency](https://github.com/nathan-rabet/PEPITAS-Cryptocurrency/archive/refs/heads/master.zip).
2. Exctract the archive and go to the exctracted directory
3. Open a terminal in the current directory, type `make`
4. Go to the `build` directory
5. Execute `client.elf`, with an argument : the IP address of an existing client, or without argument if you are the first node of the network

# Some explainations about how the program works
When launched, the client will try to connect to the host you provided as an argument (if provided). In the case where no argument are given to the ELF program, the client will try to etablish a connect to a *serverdoor* (a node which have executed the program `serverdoor.elf`). A serverdoor is a program that provides IP adresses to a node in order to let him have a connection with the corresponding hosts.

*NB : serverdoors IP adresses are stored in the `HARD_CODED_ADDR` macro in `src/core/network/network.c` and may not work if Maxence and/or Nathan decide to close the non-localhost serverdoor (currently hosted in a Google Cloud instance). If you really want to use a serverdoor, you also can refactor the `HARD_CODED_ADDR` macro and then run the program `serverdoor.elf`*

If you want more information about the peer-to-peer or the validation protocol, you also can read the [P2P_PROTOCOL.md](P2P_PROTOCOL.md)) or the [VALIDATION_PROTOCOL.md](P2P_PROTOCOL.md) documentation.

Also, don't hesitate to check our Doxygen [code documentation](docs/documentation/index.html)!

## Contributors
- Nathan RABET, project leader, in charge of the blockchain and the validation protocol.
- Maxence ODEN, in charge of the networking and cryptographic part of this project.
- Souleymane SENTICI, responsible for the user interface.
- Luca SAINGIER, responsible for the web implementation.
