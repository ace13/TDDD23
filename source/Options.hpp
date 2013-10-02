#pragma once

#include <unordered_map>
#include <functional>
#include <typeindex>
#include <string>

class Application;

struct CVAR
{
    CVAR() : Flags(0), Type(typeid(void)), Opaque(nullptr), Set(nullptr), Get(nullptr) { }

    enum Flags
    {
        Default,
        Internal = 1 << 0,
        Max      = 1 << 0
    };

    int Flags;
    std::string Name;
    std::string Description;
    std::type_index Type;

    // Don't touch these
    void* Opaque;
    void* Set;
    void* Get;
};

template<typename T>
struct CVARRef
{
    CVARRef(const CVAR& cv) : Var(cv) { }
    const CVAR& Var;

    CVARRef& operator=(const T& val) { (*(std::function<void(const T&)>*)Var.Set)(val); return *this; }
    operator T() { return (*(std::function<T()>*)Var.Get)(); }
};

class Options
{
public:
    Options(Application&);
    ~Options();

    void parseARGV(int argc, char** argv);
    void loadConfig(const std::string& file);

    template<typename T>
    void registerVariable(T& t, const std::string& name, const std::string& desc = "", int flags = CVAR::Default);
    template<typename T>
    void registerProperty(const std::function<void(const T&)>& set, const std::function<T(void)>& get, const std::string& name, const std::string& desc = "", int flags = CVAR::Default);
    template<typename T>
    void addVariable(const std::string& name, const T& def, const std::string& desc = "", int flags = CVAR::Default);
    void registerCVAR(const CVAR& t);

    const CVAR& getCVAR(const std::string& name) const;
    template<typename T>
    CVARRef<T> operator[](const std::string& name) const;
    template<typename T>
    T get(const std::string& name) const;
    template<typename T>
    void set(const std::string& name, const T&) const;

private:
    Application& mApp;
    std::unordered_map<std::string, CVAR> mStored;
};

template<typename T>
void Options::registerVariable(T& t, const std::string& name, const std::string& desc, int flags)
{
    CVAR cv;
    cv.Flags = flags;
    cv.Name = name;
    cv.Description = desc;
    cv.Type = typeid(T);
    
    cv.Set = new std::function<void(const T& val)> { [t](const T& val) { t = val; } };
    cv.Get = new std::function<T()> { [t]() { return t; } };

    registerCVAR(cv);
}

template<typename T>
void Options::registerProperty(const std::function<void(const T&)>& set, const std::function<T(void)>& get, const std::string& name, const std::string& desc, int flags)
{
    CVAR cv;
    cv.Flags = flags;
    cv.Name = name;
    cv.Description = desc;
    cv.Type = typeid(T);
    
    cv.Set = new std::function<void(const T& val)>(set);
    cv.Get = new std::function<T()>(get);

    registerCVAR(cv);
}

template<typename T>
void Options::addVariable(const std::string& name, const T& def, const std::string& desc, int flags)
{
    CVAR cv;
    cv.Flags = flags | CVAR::Internal;
    cv.Name = name;
    cv.Description = desc;
    cv.Type = typeid(T);
    T* var = new T(def);
    cv.Opaque = var;
    
    cv.Set = new std::function<void(const T& val)> { [var](const T& val) { *var = val; } };
    cv.Get = new std::function<T()> { [var]() { return *var; } };

    registerCVAR(cv);
}

template<typename T>
CVARRef<T> Options::operator[](const std::string& name) const
{
    return CVARRef<T>(getCVAR(name));
}

template<typename T>
T Options::get(const std::string& name) const
{
    return (*(std::function<T()>*)mStored.at(name).Get)();
}

template<typename T>
void Options::set(const std::string& name, const T& val) const
{
    (*(std::function<void(const T&)>*)mStored.at(name).Set)(val);
}
