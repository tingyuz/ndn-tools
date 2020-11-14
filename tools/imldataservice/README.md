# ndndataservice

**ndndataservice** and **ndndataclient** are a pair of programs to respectively request and serve a single Data packet.

* **ndndataclient** is a consumer program that sends one Interest and expects one Data.
* **ndndataservice** is a producer program that serves one Data in response to an Interest.

Usage example:

1. start NFD by nfd-start
2. execute `./build/bin/ndndataservice`
3. in another terminal, execute `./build/bin/ndndataclient /ndn/dell/ep1`
