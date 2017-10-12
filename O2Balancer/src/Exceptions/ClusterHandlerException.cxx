#include "O2/Balancer/Exceptions/ClusterHandlerException.h"

using namespace O2::Balancer::Exceptions;

ClusterHandlerException::ClusterHandlerException(const std::string& msg) : AbstractException(msg){
    
}