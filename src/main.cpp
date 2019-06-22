#include <napi.h>
#include "memory.hpp"

class NProcess : public Napi::ObjectWrap<NProcess> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
      Napi::HandleScope scope(env);

      Napi::Function func = DefineClass(env, "Process", {
        InstanceMethod("reattach", &NProcess::reattach),
        InstanceMethod("attached", &NProcess::is_attached),
        InstanceMethod("base", &NProcess::get_base),
        InstanceMethod("pointer", &NProcess::eval_pointer_chain),
        InstanceMethod("readI32", &NProcess::read<int32_t>),
        InstanceMethod("readI64", &NProcess::read<int64_t>),
        InstanceMethod("readU32", &NProcess::read<uint32_t>),
        InstanceMethod("readF", &NProcess::read<float>),
        InstanceMethod("readD", &NProcess::read<double>),
        InstanceMethod("writeI32", &NProcess::write<int32_t>),
        InstanceMethod("writeI64", &NProcess::write<int64_t>),
        InstanceMethod("writeU32", &NProcess::write<uint32_t>),
        InstanceMethod("writeF", &NProcess::write<float>),
        InstanceMethod("writeD", &NProcess::write<double>),
      });

      constructor = Napi::Persistent(func);
      constructor.SuppressDestruct();

      exports.Set("Process", func);
      return exports;
    }

    NProcess (const Napi::CallbackInfo& info) : Napi::ObjectWrap<NProcess>(info) {
      Napi::Env env = info.Env();
      Napi::HandleScope scope(env);

      int length = info.Length();
      if (length != 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
      }

      process_name = info[0].As<Napi::String>();
      try {
        this->proc = new Process(process_name);
      } catch (const memory_exception& e) {
        throw Napi::Error::New(env, e.what());
      }
    }

  private:
    static Napi::FunctionReference constructor;

    Napi::Value reattach(const Napi::CallbackInfo& info) {
      Napi::Env env = info.Env();
      Napi::HandleScope scope(env);

      try {
        this->proc->attach(process_name);
      } catch (const memory_exception& e) {
        throw Napi::Error::New(env, e.what());
      }

      return Napi::Boolean::New(env, this->proc->is_attached());
    }

    Napi::Value is_attached(const Napi::CallbackInfo& info) {
      Napi::Env env = info.Env();
      Napi::HandleScope scope(env);

      return Napi::Boolean::New(env, this->proc->is_attached());
    }

    Napi::Value get_base(const Napi::CallbackInfo& info) {
      Napi::Env env = info.Env();
      Napi::HandleScope scope(env);

      return Napi::Number::New(env, this->proc->get_base());
    }
    
    Napi::Value eval_pointer_chain(const Napi::CallbackInfo& info) {
      Napi::Env env = info.Env();
      Napi::HandleScope scope(env);

      int length = info.Length();
      if (length != 1 || !info[0].IsArray()) {
        Napi::TypeError::New(env, "Array expected").ThrowAsJavaScriptException();
      }

      Napi::Array addr_p = info[0].As<Napi::Array>();

      uint32_t N = addr_p.Length();
      std::vector<uint64_t> chain;
      chain.reserve(N);
      for (auto i = 0; i < N; i++) {
        chain.push_back((uint64_t)addr_p.Get(i).As<Napi::Number>().Int64Value());
      }

      try {
        return Napi::Number::New(env, this->proc->eval_pointer_chain(chain));
      } catch (const memory_exception& e) {
        throw Napi::Error::New(env, e.what());
      }
    }

    template<typename T> Napi::Value read(const Napi::CallbackInfo& info) {
      Napi::Env env = info.Env();
      Napi::HandleScope scope(env);

      int length = info.Length();
      if (length != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
      }

      uint64_t addr = info[0].As<Napi::Number>().Int64Value();
      try {
        return Napi::Number::New(env, this->proc->read<T>(addr));
      } catch (const memory_exception& e) {
        throw Napi::Error::New(env, e.what());
      }
    }

    template<typename T> Napi::Value write(const Napi::CallbackInfo& info) {
      Napi::Env env = info.Env();
      Napi::HandleScope scope(env);

      int length = info.Length();
      if (length != 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Numbers expected").ThrowAsJavaScriptException();
      }

      uint64_t addr = info[0].As<Napi::Number>().Int64Value();
      T val = T(info[1].As<Napi::Number>()); // FloatValue();
      try {
        this->proc->write<T>(addr, val);
      } catch (const memory_exception& e) {
        throw Napi::Error::New(env, e.what());
      }
      return env.Null();
    }

    Process* proc;
    std::string process_name;
};
Napi::FunctionReference NProcess::constructor;

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  NProcess::Init(env, exports);
  return exports;
}

NODE_API_MODULE(amnesiaclapp, InitAll)