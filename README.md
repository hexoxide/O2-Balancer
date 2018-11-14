![https://travis-ci.com/hexoxide/O2-Balancer](https://api.travis-ci.com/hexoxide/O2-Balancer.svg?branch=master)
# Load balancer for ALICE
In collaboration with the Amsterdam University of Applied sciences and CERN a load balancer is being developed for the ALICE O2 project.

## Operation
The balancer is build out of three parts each with its own purposes and characteristics. 

* InformationNode
* FLP
* EPN

## Installation

```bash
if [ ! -d "build" ]; then mkdir build; fi
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j 2
```

## Testing

```bash
sudo /usr/share/zookeeper/bin/zkServer.sh start
./execute.sh
```
