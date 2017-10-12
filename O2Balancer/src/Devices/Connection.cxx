#include "O2/Balancer/Devices/Connection.h"
#include "O2/Balancer/Devices/AbstractDevice.h"
#include "O2/Balancer/Exceptions/UnimplementedException.h"


using namespace O2::Balancer;

Connection::Connection(const std::string& name, AbstractDevice* device){
    this->name = name;
    this->device = device;
}

void Connection::updateAllRateLogging(const int& logg){
    for(auto& i : this->channels){
        i.UpdateRateLogging(logg);
    }
}

void Connection::updateAllReceiveBuffer(const int& buffer){
    for(auto& i : this->channels){
        i.UpdateRcvBufSize(buffer);
    }
}

void Connection::updateAllSendBuffer(const int& buffer){
    for(auto& i : this->channels){
        i.UpdateSndBufSize(buffer);
    }
}

void Connection::updateAllSendKernelSize(const int& size){
    for(auto& i : this->channels){
        i.UpdateSndKernelSize(size);
    }
}

void Connection::updateAllReceiveKernelSize(const int& size){
    for(auto& i : this->channels){
        i.UpdateRcvKernelSize(size);
    }
}

std::string Connection::typeToString(ConnectionType type) const{
    switch(type){
        case ConnectionType::Publish:
        return "pub";
        case ConnectionType::Pull:
        return "pull";
        case ConnectionType::Subscribe:
        return "sub";
        case ConnectionType::Push:
        return "push";
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

std::vector<FairMQChannel> Connection::getChannels() const{
    return this->channels;
}

FairMQChannel Connection::addChannel(ConnectionType type, ConnectionMethod method,const std::string& ip, int port){
    FairMQChannel channel(
        this->typeToString(type),
        this->methodToString(method),
        "tcp://" + std::string(ip) + ":" + std::to_string(port)
    );
    this->channels.push_back(channel);

    return channel;
}

void Connection::updateConnection(std::shared_ptr<ClusterManager> clusterManager){
    //virtual
}

std::string Connection::getName() const{
    return this->name;
}