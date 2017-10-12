#include "O2/Balancer/Exceptions/TimeOutException.h"

using namespace O2::Balancer::Exceptions;

TimeOutException::TimeOutException(const std::string& msg) : ClusterHandlerException(msg){
    
}