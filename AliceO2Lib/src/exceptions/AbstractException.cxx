#include "O2/exceptions/AbstractException.h"

using namespace O2::Exceptions;


AbstractException::AbstractException(const std::string& message){
    this->msg = message;
}

std::string AbstractException::getMessage() const{
    return this->msg;
}