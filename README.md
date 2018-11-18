![https://travis-ci.com/hexoxide/O2-Balancer](https://api.travis-ci.com/hexoxide/O2-Balancer.svg?branch=fixing)
# Load balancer for ALICE
In collaboration with the Amsterdam University of Applied sciences and CERN a load balancer is being developed for the ALICE O2 project.

## Operation
The balancer is build out of three parts each with its own purposes and characteristics. 

| Node                 | Purpose             | Status       |
|----------------------|---------------------|--------------|
| InformationNode      | C&C + Orchestration | Broken AF    |
| FirstLineProccessing | Push data           | Mostly works |
| EventProcessingNode  | Pull data           | Working      |

### Configuration
All binaries share some configuration parameters but have specific one's as well. Some of these parameters need to be supplied as commandline arguments and others are read from a yaml configuration file. Some of the arguments can be specified both as an argument and in the configuration file, it is unclear which value will be applied at that point.

| Argument | Purpose                          | ICN | FLP | EPN |
|----------|----------------------------------|-----|-----|-----|
| --ip     | Likely the ip address to bind to |   X |  X  |  X  |
|          |                                  |     |     |     |
|          |                                  |     |     |     |

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

## Debugging
Start individual applications of the O2 balancer using gdb and be sure to pipe the output to a file or null.
```bash
gdb -tui FILE
run > output.log
```
