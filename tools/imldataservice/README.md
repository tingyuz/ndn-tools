# ndndataservice

**ndndataservice** and **ndndataclient** are a pair of programs to respectively request and serve a single Data packet.

* **ndndataclient** is a consumer program that sends one Interest and expects one Data.
* **ndndataservice** is a producer program that serves one Data in response to an Interest.

Usage example:

1. start NFD 
2. execute `./build/bin/ndndataservice`
3. in another terminal, execute `./build/bin/ndndataclient`, the dataclient is listening on http://localhost:8080/interest and http://localhost:8080/locality
4. use curl to send resource request curl http://localhost:8080/interest/ or http://localhost:8080/locality withi json body to include names of the nameddata
