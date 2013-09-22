#include "ScriptManager.hpp"
#include <angelscript.h>
#include <iostream>

ScriptManager::ScriptManager()
{

}

ScriptManager::~ScriptManager()
{

}

void ScriptManager::init()
{
    mEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

    ///\TODO Register the AS types.
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

void ScriptManager::exceptionCallback(asIScriptContext* ctx)
{
    std::cerr << "Ran into exception while executing Angelscript!" << std::endl;
}
