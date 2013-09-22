#pragma once

#include <vector>

class asIScriptContext;
class asIScriptEngine;

#define asAssert(r) assert((r) > 0)

/** A basic manager to keep track of scripts
 */
class ScriptManager
{
public:
    ScriptManager();
    ~ScriptManager();

    void init();

    asIScriptContext* getContext();
    void returnContext(asIScriptContext* ctx);

private:
    /// Called by the script engine when a context fails execution.
    void exceptionCallback(asIScriptContext*);

    asIScriptEngine* mEngine;
    std::vector<asIScriptContext*> mContexts;
};
