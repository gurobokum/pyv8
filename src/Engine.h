#pragma once

#include <string>

#include <boost/shared_ptr.hpp>

#include "Context.h"

class CScript;

typedef boost::shared_ptr<CScript> CScriptPtr;

class CEngine
{  
  CContextPtr m_context;
protected:
  static void ReportFatalError(const char* location, const char* message);
  static void ReportMessage(v8::Handle<v8::Message> message, v8::Handle<v8::Value> data);  
public:
  CEngine(py::object global = py::object()) 
    : m_context(CContext::Create(global))
  {

  }
  CEngine(CContextPtr context) 
    : m_context(context)
  {

  }

  CContextPtr GetContext(void) { return m_context; }

  CScriptPtr Compile(const std::string& src);
  CJavascriptObjectPtr Execute(const std::string& src);

  void RaiseError(v8::TryCatch& try_catch);
public:
  static void Expose(void);

  static const std::string GetVersion(void) { return v8::V8::GetVersion(); }

  CJavascriptObjectPtr ExecuteScript(v8::Persistent<v8::Script>& m_script);
};

class CScript
{
  CEngine& m_engine;
  const std::string m_source;

  v8::Persistent<v8::Script> m_script;  
public:
  CScript(CEngine& engine, const std::string& source, const v8::Persistent<v8::Script>& script) 
    : m_engine(engine), m_source(source), m_script(script)
  {

  }
  ~CScript()
  {
    m_script.Dispose();
  }

  const std::string GetSource(void) const { return m_source; }

  CJavascriptObjectPtr Run(void) { return m_engine.ExecuteScript(m_script); }
};
