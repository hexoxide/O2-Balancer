#include "O2/exceptions/UnimplementedException.h"

using namespace O2::Exceptions;


UnimplementedException::UnimplementedException(const std::string& msg) : AbstractException(msg){
    
}