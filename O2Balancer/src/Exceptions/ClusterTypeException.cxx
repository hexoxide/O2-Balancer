#include "O2/Balancer/Exceptions/ClusterTypeException.h"

using namespace O2::Balancer::Exceptions;

ClusterTypeException::ClusterTypeException(const std::string& msg) : ClusterHandlerException(msg){
    
}