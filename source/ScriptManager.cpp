#include "ScriptManager.hpp"
#include <angelscript.h>
#include <iostream>

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
    template<typename T>
    void print(const T& val) { std::cout << val << std::endl; }
    void print(void) { std::cout << std::endl; }
};

ScriptManager::ScriptManager()
{

}

ScriptManager::~ScriptManager()
{

}

void ScriptManager::init()
{
    mEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

    RegisterScriptAny(mEngine);
    RegisterScriptArray(mEngine, true);
    RegisterScriptMath(mEngine);
    RegisterScriptMathComplex(mEngine);
    RegisterStdString(mEngine);
    RegisterScriptDictionary(mEngine);
    RegisterStdStringUtils(mEngine);

    int r = 0;
    r = mEngine->RegisterGlobalFunction("void print()", asFUNCTIONPR(print, (), void), asCALL_CDECL); asAssert(r);
    r = mEngine->RegisterGlobalFunction("void print(int &in)", asFUNCTIONPR(print, (const int&), void), asCALL_CDECL); asAssert(r);
    r = mEngine->RegisterGlobalFunction("void print(float &in)", asFUNCTIONPR(print, (const float&), void), asCALL_CDECL); asAssert(r);
    r = mEngine->RegisterGlobalFunction("void print(double &in)", asFUNCTIONPR(print, (const double&), void), asCALL_CDECL); asAssert(r);
    r = mEngine->RegisterGlobalFunction("void print(string &in)", asFUNCTIONPR(print, (const std::string&), void), asCALL_CDECL); asAssert(r);

    CScriptBuilder build;
    build.StartNewModule(mEngine, "DebugModule");
    build.AddSectionFromMemory("void _grab() { print(\"=>\"); } \
        void _grab(int i) { print(\"=> \" + i); } void _grab(float f) { print(\"=> \" + f); } \
        void _grab(double d) { print(\"=> \" + d); } void _grab(string s) { print(\"=> \" + s); }", "_grab");
    build.BuildModule();
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
    int r = ctx->Unprepare(); asAssert(r);
}

void ScriptManager::runString(const std::string& str)
{
    auto ctx = getContext();

    std::cout << "Script> " << str << std::endl;

    std::string cleaned = str;

    if (cleaned.back() == ';')
        cleaned.erase(cleaned.length() - 1, 1);

    cleaned = "_grab(" + cleaned + ");";

    ExecuteString(mEngine, cleaned.c_str(), mEngine->GetModule("DebugModule"), ctx);

    returnContext(ctx);
}

void ScriptManager::exceptionCallback(asIScriptContext* ctx)
{
    std::cerr << "Ran into an exception while executing Angelscript!" << std::endl;

    PrintException(ctx, true);
}
