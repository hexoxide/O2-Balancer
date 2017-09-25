// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_CONNECTION_H
#define O2_CONNECTION_H

#include <string>
#include <vector>
#include <FairMQChannel.h>

namespace O2{
    enum class ConnectionType{
        Publish,
        Pull
    };

    enum class ConnectionMethod{
        Bind,
        Connect
    };

    class Connection{
        std::string typeToString(ConnectionType type) const;
        std::string methodToString(ConnectionMethod method) const;
        std::string name;
        std::vector<FairMQChannel> channels;
    public:
        void addChannel(ConnectionType type, ConnectionMethod method, int port);
        Connection(const std::string& name);
        std::string getName() const;
    };
}

#endif