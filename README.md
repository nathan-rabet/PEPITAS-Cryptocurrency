![Pepitas image](https://user-images.githubusercontent.com/13369175/121909610-60bb2800-cd2e-11eb-9510-6aef81d91b16.png)
# PEPITAS, C-based cryptocurrency

[![Builder](https://github.com/nathan-rabet/PEPITAS-Cryptocurrency/actions/workflows/builder.yml/badge.svg)](https://github.com/nathan-rabet/PEPITAS-Cryptocurrency/actions/workflows/builder.yml)

PEPITAS is an EPITA project which was done during the last semester of the preparatory cycle.
This cryptocurrency is based on the *proof of stake*, a newer and more eco-friendly validation consensus (used in Etherum 2.0).

With PEPITAS, you can do whatever a modern proof of stake based money can also do :
- Send money
- Receive money
- Invest on the stake
- Validate transactions
- Earn transactions fees

All of these features are obviously based on a 2048-bits RSA protocol.

## Requirements
- A Linux system (Ubuntu, Arch,...)
- GNU Make
- OpenSSL
- GTK 

## Installation & execution
1. Download the last version of the project : [PEPITAS-Cryptocurrency](https://github.com/nathan-rabet/PEPITAS-Cryptocurrency/archive/refs/heads/master.zip).
2. Exctract the archive
4. Open a terminal in the exctracted directory and type `make` (or `make client` if you just want to execute the client)
5. Go to the `build` directory
6. Execute `client.elf`, with an argument : the IP address of an existing client, or without argument if you are the first node of the network

## Some explainations about how the client works
When launched, the client will try to connect to the host you provided as an argument (if provided). In the case where no argument are given to the ELF program, the client will try to etablish a connect with a *serverdoor* (a node which have executed the program `serverdoor.elf`). A serverdoor is a program that provides IP adresses to a node in order to let him have a connection with the corresponding hosts.

*NB : serverdoors IP adresses are stored in the `HARD_CODED_ADDR` constant in [src/core/network/network.c](src/core/network/network.c) and may not work if Maxence and/or Nathan decide to close the non-localhost serverdoor (currently hosted in a Google Cloud instance). If you really want to use a serverdoor, you also can refactor the `HARD_CODED_ADDR` constant and then run the program `serverdoor.elf`*

## More information
If you want more information about how the peer-to-peer network or the validation protocol works, you can also read the [P2P_PROTOCOL.md](P2P_PROTOCOL.md) or the [VALIDATION_PROTOCOL.md](VALIDATION_PROTOCOL.md) documentation.

Also, don't hesitate to check our Doxygen code documentation ([web](https://nathan-rabet.github.io/PEPITAS-Cryptocurrency/documentation/)/[pdf](docs/manual.pdf)).

## Contributors
- **Nathan RABET**, project leader, in charge of the validation protocol and the blockchain implementation.
- **Maxence ODEN**, in charge of the networking, the multithreading and the cryptographic part of this project.
- **Souleymane SENTICI**, responsible for the user interface.
- **Luca SAINGIER**, responsible for the web implementation.
