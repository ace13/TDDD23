#pragma once

#include <unordered_map>
#include <typeindex>

class Application;

struct CVAR
{
    enum Flags
    {
        Invalid = 0,
        Max
    };

    int Flags;
    std::string Name;
    std::type_index Type;
    void* Address;
};

class Options
{
public:
    Options(Application&);
    ~Options();

    void parseARGV(int argc, char** argv);
    void loadConfig(const std::string& file);

    template<typename T>
    void registerVariable(T& t, const std::string& name, int flags = CVAR::Flags::Invalid);
    void registerCVAR(const CVAR& t);

private:
    Application& mApp;
    std::unordered_map<std::string, CVAR> mStored;
};

template<typename T>
Options::registerVariable(T& t, const std::string& name, int flags)
{
    static_assert( typeid(t) == typeid(bool) || typeid(t) == typeid(int) || typeid(t) == typeid(float) || typeid(t) == typeid(std::string) );

    CVAR cv;
    cv.Flags = flags;
    cv.Name = name;
    cv.Type = typeid(T);
    cv.Address = &t;

    registerCVAR(cv);
}