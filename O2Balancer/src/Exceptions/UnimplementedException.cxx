#include "O2/Balancer/Exceptions/UnimplementedException.h"

using namespace O2::Balancer::Exceptions;


UnimplementedException::UnimplementedException(const std::string& msg) : AbstractException(msg){
    
}