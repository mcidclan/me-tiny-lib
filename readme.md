# TinyMe, no PRX

A tiny PSP Media Engine library using **LibPspExploit**, which does not require a PRX to run code over the ME

## Overview
`main.cpp` serves as a sample that runs a counter over the Media Engine.

## Requirements
To use this library, you could include a compiled version of `LibPspExploit` with its header files in the `vendor` directory. Use the latest version provided in the repository's release section.
Alternatively, you could place the `.c` files related to `LibPspExploit` directly in the `vendor` folder. Just use `make clean; make` to compile the project in both cases.

## Special Thanks

This project wouldn't have been possible without the resources from the PSP homebrew community, which served as valuable sources of knowledge.
Thanks to **crazyc** from ps2dev.org, without whom the use of the Media Engine in the community would be far more difficult.
Thanks to `qwikrazor87`, `Davee`, `CelesteBlue` and `Acid_Snake` for **LibPspExploit**
Thanks to all developers and contributors who have kept the scene alive and to those who continue to do so.

Indispensable resources:
- [uofw on GitHub](https://github.com/uofw/uofw)
- [psp wiki on PSDevWiki](https://www.psdevwiki.com/psp/)
- [LibPspExploit on GitHub](https://github.com/PSP-Archive/LibPspExploit/)
- [pspdev on GitHub](https://github.com/pspdev)

*m-c/d*
