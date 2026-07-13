# KDB-X Modules

## Introduction

This repository contains a collection of KDB-X modules implemented in C, built with a remarkably simple cross-platform build system.

The following is a table of the existing modules and their supported platforms.

| Module | Description  | Mac Intel | Mac Arm | Linux Intel | Linux Arm | Windows Intel |
| -------| ------------ | --------- | ------- | ----------- | --------- | ------------- |
| [bit](bit/README.md)         | bitwise operators   | ✅ | ✅ | ✅ | ✅ | ✅ |
| [flock](flock/README.md)     | advisory locks      | ✅ | ✅ | ✅ | ✅ | ✅ |
| [udp](udp/README.md)         | UDP messaging       | ✅ | ✅ | ✅ | ✅ | ✅ |
| [selectk](selectk/README.md) | selection algorithm | ✅ | ✅ | ✅ | ✅ | ✅ |

All Linux binaries are compatible with GLIBC 2.12 and above, i.e. they work even on very old distributions like Centos 6 or Red Hat 6.

## Installation

Download the release package from [releases](https://github.com/atdotsch/kdb-x/releases) and unzip it in your KDB-X `mod` directory.

## Quick Start

```q
q) // load all module functions in the .q namespace and use them like built-in functions
q) .q,:use`dot
q) // load only a specific module
q) .q,:use`dot.bit
q) // or load it in a dedicated namespace, this will only allow prefix notation
q) .bit:use`dot.bit
```

Please refer to the documentation of the individual modules for more information.

## Building

All modules use the same containerized build system based on the wonderful [Zig](https://ziglang.org/) cross-compiler. Shout out to [Andrew Kelley](https://github.com/andrewrk) for creating Zig and making multiplatform C development a breeze.

The only prerequisite for building the project is Docker, the build environment is created inside a Docker container.
Alternatively, you can install everyu build dependency listed in [Dockerfile](./Dockerfile) on a Linux machine and build any module with `make` from its own subdirectory.

To build all 64bit binaries:
```sh
 $ sh mk.sh
```
To build all 32bit binaries:
```sh
 $ sh mk.sh all32
```
To build only a specific module for a specific target, like x86_64 Linux:
```sh
 $ sh mk0.sh <module> init.li64.so
```

## Donations

If you use these modules, please consider supporting the author with a donation in any currency/coin:

- Revolut: [@andrasdotsch](https://revolut.me/andrasdotsch)
- Bitcoin: `bc1qfanp6jt8rnrw9zjpc4pwpsd3x552rathjrmecw`
- Ethereum (and L2/EVM chains): `0xf780b169E2177d13938355bF5abE796575C1003E`
- XRP: `rPthpv7Ex7CGPc28rVKtfzazgM2MxcketY`
- Solana: `21gwoU51YPRD29vpsRYKyvNJ6xEtJSXdM7LPXrG63RtA`
- Dogecoin: `D8eSvpNkaAfhyFhxyXcaTXsVAMgERsAico`
- Zcash: `t1L1ncvwaCQhp6yziDtcrWrd7vquioswNxC`

## License

MIT