#ifndef O2_PROGRAM_OPTIONS_H
#define O2_PROGRAM_OPTIONS_H

#include <boost/program_options.hpp>

namespace O2{
    boost::program_options::variables_map AddO2Options(boost::program_options::options_description&, int argc, char** argv);
}

#endif