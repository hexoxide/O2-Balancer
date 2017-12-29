# O2 Balancer
A shared library with all the common functions/classes and utilities.

## Table of contents
* [Introduction] (#Introduction)
* [Exception model](#ExceptionModel)

## Introduction
The EPNs, FLPs and Information Node software all make use of the same constructs.
This library contains all the abstractions and tools shared among the derived executables.
Functions of the library:

- Exception handling
- Loading of configuration files
- Communication with Zookeeper
- Narrowing the usage of FairMQ

## Exception Model
The
