#pragma once

#include <vector>
#include <cassert>
#include <string>

class asIScriptContext;
class asIScriptEngine;
class asSMessageInfo;
class Application;

#define asAssert(r) assert((r) >= 0)

/** A basic manager to keep track of scripts
 */
class ScriptManager
{
public:
    ScriptManager(Application&);
    ~ScriptManager();

    void init();

    asIScriptContext* getContext();
    void returnContext(asIScriptContext* ctx);

    void runString(const std::string& code);

private:
    /// Called by the script engine when a context fails execution.
    void exceptionCallback(asIScriptContext*);
    void messageCallback(const asSMessageInfo *msg);

    Application& mApp;
    asIScriptEngine* mEngine;
    std::vector<asIScriptContext*> mContexts;
};
