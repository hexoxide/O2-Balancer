#include "O2/exceptions/InitException.h"

using namespace O2::Exceptions;

InitException::InitException(const std::string& msg) : AbstractException(msg){
    
}