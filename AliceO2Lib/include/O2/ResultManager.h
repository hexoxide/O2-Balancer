// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_RESULT_MANAGER_H
#define O2_RESULT_MANAGER_H

#include <string>
#include <fstream>
#include <chrono>
namespace O2{
    class ResultManager{
    private:
        std::ofstream output;
        std::chrono::time_point<std::chrono::system_clock> last;
    public:
        ResultManager(const std::string& fileName = "results.csv");
        void addTimeFrame(uint16_t frame);

        ~ResultManager();
    };
}

#endif