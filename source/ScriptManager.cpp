#include "ScriptManager.hpp"
#include "Application.hpp"
#include <angelscript.h>
#include <sstream>

#include <scriptany/scriptany.h>
#include <scriptarray/scriptarray.h>
#include <scriptdictionary/scriptdictionary.h>
#include <scripthelper/scripthelper.h>
#include <scriptmath/scriptmath.h>
#include <scriptmath/scriptmathcomplex.h>
#include <scriptstdstring/scriptstdstring.h>

#include <scriptbuilder/scriptbuilder.h>

// Anonymous AS functions
namespace
{
    Logger* glog;
    template<typename T>
    void print(const T& val) { std::ostringstream oss; oss << val; glog->log(oss.str(), Logger::Script); }
    void print() { glog->log("", Logger::Script); }
};

ScriptManager::ScriptManager(Application& app) : mApp(app), mEngine(nullptr)
{

}

ScriptManager::~ScriptManager()
{

}

void ScriptManager::init()
{
    glog = &mApp.getLogger();
    mEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    mEngine->SetMessageCallback(asMETHOD(ScriptManager, messageCallback), this, asCALL_THISCALL);

    // Base types
    RegisterScriptAny(mEngine);
    RegisterScriptArray(mEngine, true);
    RegisterScriptMath(mEngine);
    RegisterScriptMathComplex(mEngine);
    RegisterStdString(mEngine);
    RegisterScriptDictionary(mEngine);
    RegisterStdStringUtils(mEngine);

    // Global functions
    int r = 0;
    r = mEngine->RegisterGlobalFunction("void print()", asFUNCTIONPR(print, (), void), asCALL_CDECL); asAssert(r);
    r = mEngine->RegisterGlobalFunction("void print(int &in)", asFUNCTIONPR(print, (const int&), void), asCALL_CDECL); asAssert(r);
    r = mEngine->RegisterGlobalFunction("void print(float &in)", asFUNCTIONPR(print, (const float&), void), asCALL_CDECL); asAssert(r);
    r = mEngine->RegisterGlobalFunction("void print(double &in)", asFUNCTIONPR(print, (const double&), void), asCALL_CDECL); asAssert(r);
    r = mEngine->RegisterGlobalFunction("void print(string &in)", asFUNCTIONPR(print, (const std::string&), void), asCALL_CDECL); asAssert(r);

    // Debug things
    /*
    CScriptBuilder build;
    r = build.StartNewModule(mEngine, "DebugModule"); asAssert(r);
    r = build.AddSectionFromMemory("void _grab() { };\n\
void _grab(int i) { print(\"=> \" + i); };\n\
void _grab(float f) { print(\"=> \" + f); };\n\
void _grab(double d) { print(\"=> \" + d); };\n\
void _grab(string s) { print(\"=> \" + s); };\n", "_grab"); asAssert(r);
    r = build.BuildModule(); asAssert(r);
    */
}

asIScriptContext* ScriptManager::getContext()
{
    asIScriptContext* ctx = nullptr;

    if (mContexts.empty())
    {
        ctx = mEngine->CreateContext();
        asAssert(ctx->SetExceptionCallback(asMETHOD(ScriptManager, exceptionCallback), this, asCALL_THISCALL));
    }
    else
    {
        ctx = mContexts.back();
        mContexts.pop_back();
    }

    return ctx;
}

void ScriptManager::returnContext(asIScriptContext* ctx)
{
    int r = ctx->Unprepare();
    if (r < 0) // If it fails to be unprepared, then let it go.
        ctx->Release();
    else
        mContexts.push_back(ctx);
}

void ScriptManager::runString(const std::string& str)
{
    auto ctx = getContext();

    mApp.getLogger().log("Executing '%s'", Logger::Info, str.c_str());

    std::string cleaned = str;

    if (cleaned.back() == ';')
        cleaned.erase(cleaned.length() - 1, 1);

    int r = ExecuteString(mEngine, cleaned.c_str(), mEngine->GetModule("DebugModule"), ctx);
    if (r < 0)
        mApp.getLogger().log("Failed to execute string, check your syntax.", Logger::Info);

    returnContext(ctx);
}

void ScriptManager::exceptionCallback(asIScriptContext* ctx)
{
    mApp.getLogger().log("Ran into an exception while executing Angelscript!", Logger::Warning);

    PrintException(ctx, true);
}

void ScriptManager::messageCallback(const asSMessageInfo *msg)
{
     Logger::Level lvl = Logger::Error;

     if (msg->type == asMSGTYPE_WARNING)
         lvl = Logger::Warning;
     else if (msg->type == asMSGTYPE_INFORMATION)
         lvl = Logger::Info;

     mApp.getLogger().log("%s:%d:%d: %s", lvl, msg->section, msg->row, msg->col, msg->message);
}