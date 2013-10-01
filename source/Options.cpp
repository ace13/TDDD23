#include "Options.hpp"
#include "Application.hpp"

Options::Options(Application& app) : mApp(app)
{

}

Options::~Options()
{

}


void Options::parseARGV(int argc, char** argv)
{

}

void Options::loadConfig(const std::string& file)
{

}


void Options::registerCVAR(const CVAR& t)
{
    mStored[t.Name] = t;
}
