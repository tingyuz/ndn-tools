# IML dataservice Build Instructions

This document describes how to build and install ndndataservice and ndndataclient.

## Prerequisites

-  Install the [ndn-cxx](https://named-data.net/doc/ndn-cxx/current/) library and its prerequisites.
   Please see [Getting Started with ndn-cxx](https://named-data.net/doc/ndn-cxx/current/INSTALL.html)
   for how to install ndn-cxx.
   Note: If you have installed ndn-cxx from a binary package, please make sure development headers
   are installed (e.g., if using Ubuntu PPA, the `libndn-cxx-dev` package is needed).

-  `libserved`

-  boost library 1.65.1 ( the higher version may cause failure)

## Build Steps

Type the following commands in the source directory:

    ./waf configure
    ./waf
    sudo ./waf install

To uninstall:

    sudo ./waf uninstall
