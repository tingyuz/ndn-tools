# IML Dataservice

**IML dataservice** is a project to utilize NDN(Named Data Network) to provide Data service for IML.

* **ndndataclient** is a consumer program that monitor http requests on port 8080 and relay Interests to ndndataservice.
* **ndndataservice** is a producer program that serves Data in response to an Interest.
* An **interest** is a request defined by NDN to ask for data.


## Installation

See [`INSTALL.md`](INSTALL.md) for build instructions.

## License

IML dataservice is an open source project licensed under the GPL version 3.
See [`COPYING.md`](COPYING.md) for more information.
