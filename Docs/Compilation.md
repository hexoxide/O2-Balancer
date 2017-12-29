# Compiling O2-Balancer
The software is dependent on the following software:
- FairRoot
- ZooKeeper
- YAML-CPP

Since the load balancer runs on CentOS 7, the installation procedure will assume this system is used.

# Step 1: Install FairRoot:
## Install the dependencies
```bash
yum install cmake gcc gcc-c++ gcc-gfortran make patch sed \
  libX11-devel libXft-devel libXpm-devel libXext-devel \
  libXmu-devel mesa-libGLU-devel mesa-libGL-devel ncurses-devel \
  curl curl-devel bzip2 bzip2-devel gzip unzip tar \
  expat-devel subversion git flex bison imake redhat-lsb-core python-devel \
  libxml2-devel wget openssl-devel krb5-devel \
    automake autoconf libtool which cmake

```

## Download FairSoft
```bash
    git clone https://github.com/FairRootGroup/FairSoft.git
    cd FairSoft
    sh configure.sh
```
Follow the instructions, although you don't need the python libraries or GEANT. This library only makes use of FairMQ.
Make sure you place the binaries in a place where you can easily find them.
To make sure your FairSoft is findable, set the export :
```bash
# Replace the place where you installed FairSoft
export SIMPATH=~/.fairsoft/FairSoft/
```

## Download and compile FairRoot
```bash
    git clone https://github.com/FairRootGroup/FairRoot.git
    cd FairRoot
    mkdir build
    cd build
    cmake ..
    make
```
Again make sure it's findable and if done set the path:

```bash
# Replace the path with your own place where you installed the directory
export FAIRROOTPATH=~/.fairsoft/FairRoot
```

# Step 2 install Zookeeper
Unfortunately Centos doesn't come automatically with Zookeeper repo, luckily you can setup a third party.
```bash
    yum install https://archive.cloudera.com/cdh5/one-click-install/redhat/7/x86_64/cloudera-cdh-5-0.x86_64.rpm
    yum install zookeeper

```
## Step 3 install yaml-cpp
Finally you need to install yaml cpp.
```bash
    yum install yaml-cpp-devel
```

## Step 4 Compile source:
```bash
    git clone https://github.com/valvy/O2-Balancer
    cd O2-Balancer
    mkdir build
    cd build
    cmake ..
    make
```