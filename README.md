libhooya
========

Shared C++ library providing connectivity to the [HooYa!](https://hooya.org) P2P
/ Kademlia network and exposing the infrastructure to tag and organize images
locally.

Executive Summary
-----------------

HooYa! is a P2P community providing persistent image storage and
community-sourced metadata tagging. Nodes on the HooYa! Kademlia network need to
communicate in a certain way to reach consensus and efficiently route search
queries. This library (1) provides interfaces to the network and (2) provides
structure to backend data which can be leveraged to create a functional node.

`hooyad` is the reference implementation of a HooYa! network node; it relies
heavily on this library which provides the canonical netcode and backend
structures.

Installation
------------

libhooya aims to have minimal dependencies. Below are the only libraries it
depends on:

- [OpenCV](https://opencv.org/)
- [Botan](https://botan.randombit.net/)

libhooya builds with CMake. To build libhooya you'll at least need these
packages:

- [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)
- [CMake](https://cmake.org)

If you're interested in building documentation you'll need:

- [Doxygen](https://www.doxygen.nl/index.html)

Install from source like this:

```
git clone 'https://github.com/hooya-network/libhooya'
cd libhooya
mkdir build; cd build
cmake ..
make
# (Optional) Build and run unit-tests
make check
# (Optional) Build documentation
make doc
# Install
make install
```

Unit-tests demonstrate typical usage of a given class.

Contributing
------------

Contribute code here or ideas via our [Discord](https://discord.gg/yFPqv8ZNNe).
All unit-tests on any PR should pass for it to be considered for merge (run
these with `make check`).

All development in this repository is done on HEAD.

License
-------

MIT License (available in the source tree as `/LICENSE`)
