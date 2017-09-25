#include "O2/Connection.h"
#include "O2/exceptions/UnimplementedException.h"
using namespace O2;

Connection::Connection(const std::string& name){
    this->name = name;
}

std::string Connection::typeToString(ConnectionType type) const{
    switch(type){
        case ConnectionType::Publish:
        return "publish";
        case ConnectionType::Pull:
        return "pull";
        default:
        throw Exceptions::UnimplementedException("typeToString doesn't implement this case");
    }
}

std::string Connection::methodToString(ConnectionMethod method) const{
    switch(method){
        case ConnectionMethod::Bind:
        return "bind";
        case ConnectionMethod::Connect:
        return "connect";
        default:
        throw Exceptions::UnimplementedException("MethodToString doesn't implement this case");
    }
}

void Connection::addChannel(ConnectionType type, ConnectionMethod method, int port){
    //std::string stype = this->typeToString(type);
    //std::string smethod = this->methodToString(method);
    this->channels.push_back(
        FairMQChannel(
            this->typeToString(type),
            this->methodToString(method),
            std::string("tcp://127.0.0.1:") + std::to_string(port)
        )
    );

}

std::string Connection::getName() const{
    return this->name;
}