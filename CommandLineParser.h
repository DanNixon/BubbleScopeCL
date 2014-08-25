#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include "Configuration.h"

#include <iostream>
#include <boost/program_options.hpp>

class CommandLineParser
{
  public:
    CommandLineParser();
    ~CommandLineParser();

    void parse(int argc, char **argv);
    void printUsage(std::ostream &stream);

    BubbleScopeCaptureParams getCaptureParams();
    BubbleScopeUnwrapParams getUnwrapParams();

    bool helpWanted();
    
  private:
    BubbleScopeCaptureParams *m_captureParams;
    BubbleScopeUnwrapParams *m_unwrapParams;

    bool m_helpWanted;

    boost::program_options::options_description m_generalOptions;
    boost::program_options::options_description m_captureOptions;
    boost::program_options::options_description m_unwrapOptions;
    boost::program_options::options_description m_outputOptions;
};

#endif
