# Meko Test Suite

This is a safe and simple to use multithreaded C++ unit testing framework.

## Features

- A beautiful customizable frontend
- Application breaking errors in tests won't influence the main-process
- Easy to use
- Parallel test execution
- Memory-leak-proof (if you follow the basic guidelines) and excessively tested using valgrind and gdb

## Building

Enter the `src` directory and use `make` to build it. (It's actually that simple!)

This will produce a `MekoTestSuite.a` that can be used in your application.\
Also all `src/Examples` will be built, I recommend checking those.\
(the `Makefile` is pretty simple, you can use your own ways of building it if you desire.)

## Requirements

All you need is an up to date Linux system, the `g++` compiler and support for the `--std=c++2a` flag. (if you manage to build it with another tool, that's great. Tell me about it!)

## Usage

check the `src/Examples` directory!

## Showcase

![Test Execution](example.gif?raw=true "Test Execution")

## about Meko

Meko is a project currently in development by me.

I recently finished the part responsible for running unit-tests within C++ and wanted to share it (the part you are currently looking at).

More info about Meko and what it's capable of will be shared in the future.

## Planed features for the TestSuite

- vscode integration
- automated test-re-run based on what code has changed (implemented in `Meko`)
- timeout for test execution (implemented for all of `Meko::Exec::Run`)

## License

I'm not a "huge complicated license"-liking guy, so:

You are allowed to use the code and the library and binaries compiled within this repository for your projects under the following simple rules:
- leave a link in an appropriate file in your project (eg. the `Readme.md`, `Dependecies.md`, etc.)
- if you earn (or want to earn) money with that project: get in touch with me for further details!
- if you change the source-files within the `src/Meko` directory and want to share that further: get in touch with me! I'm eager to see your improvements.
- only distribute the source-files and portions of it using this repository!

how to get in touch with me?
e-mail me at niklas.becker.dev@gmail.com
