#include "O2/Balancer/Exceptions/AbstractException.h"

using namespace O2::Balancer::Exceptions;


AbstractException::AbstractException(const std::string& message){
    this->msg = message;
}

std::string AbstractException::getMessage() const{
    return this->msg;
}