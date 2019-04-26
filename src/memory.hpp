#pragma once

#include <windows.h>
#include <Psapi.h>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdint>
#include <tuple>
#include "tinyformat.h"

bool lower_equals(const std::string& a, const std::string& b) {
  auto it_a = a.begin();
  auto it_b = b.begin();
  while (it_a != a.end() && it_b != b.end()) {
    if (tolower(*it_a) != tolower(*it_b)) return false;
    ++it_a; ++it_b;
  }
  return true;
}

std::tuple<DWORD, HMODULE, MODULEINFO> find_process (const std::string& name) {
  DWORD ret_pid = -1;
  HMODULE ret_base = 0;
  MODULEINFO ret_info;

  DWORD lpid_process[256];
  unsigned long cb_needed, count;
  HMODULE h_module[64];
  char modname[30];

  EnumProcesses(lpid_process, sizeof(lpid_process), &cb_needed);
  int nReturned = cb_needed / sizeof(cb_needed);

  for (int i = 0; i < nReturned; i++) {
    auto pid = lpid_process[i];
    auto h_proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);
    if(!h_proc) continue;

    EnumProcessModules(h_proc, h_module, sizeof(h_module), &count);
    GetModuleBaseNameA(h_proc, h_module[0], modname, sizeof(modname));

    std::string procname(modname);
    if (lower_equals(procname, name)) {
      GetModuleInformation(h_proc, h_module[0], &ret_info, sizeof(ret_info));
      ret_pid = pid;
      ret_base = h_module[0];
    }

    for (int j = 0; j < 30; j++) modname[j] = 0;
    CloseHandle(h_proc);
  }

  return std::make_tuple(ret_pid, ret_base, ret_info);
}

class memory_exception : public std::domain_error {
  using std::domain_error::domain_error;
};

class Process {
  private:
    DWORD pid;
    uint64_t base;
    MODULEINFO info;
    HANDLE proc;

  public:
    Process () = default;
    Process (const std::string& name) {
      attach(name);
    }
    Process (const Process& p) = default; // : pid(p.pid), base(p.base), info(p.info), proc(p.proc) {}

    const uint64_t get_base() const { return base; }

    void attach (const std::string& name) {

      if (proc != nullptr) {
        CloseHandle(nullptr);
      }

      std::tuple<DWORD, HMODULE, MODULEINFO> m = find_process(name);
      pid = std::get<0>(m);
      base = (uint64_t)std::get<1>(m);
      info = std::get<2>(m);
      proc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

      if (!proc) {
        throw memory_exception("OpenProcess");
      }
    }

    void assert_attached () const {
      if (!is_attached()) {
        throw memory_exception("Process is not attached!");
      }
    };

    bool is_attached () const {
      DWORD dw;
      GetExitCodeProcess(proc, &dw);
      return proc != nullptr && dw == STILL_ACTIVE;
    };

    template<typename T> T read (uint64_t addr) {
      T dest;
      uint64_t bytes_read;
      auto ret = ReadProcessMemory(proc, (void*)addr, &dest, sizeof(T), &bytes_read);
      if (ret == 0) {
        throw memory_exception(tfm::format("ReadProcessMemory: %x", GetLastError()));
      }
      return dest;
    }

    template<typename T> void write (uint64_t addr, T data) {
      uint64_t bytes_written;
      auto ret = WriteProcessMemory (proc, (void*)addr, &data, sizeof(T), &bytes_written);
      if (ret == 0) {
        throw memory_exception(tfm::format("WriteProcessMemory: %x", GetLastError()));
      }
    }

    uint64_t eval_pointer_chain (std::vector<uint64_t> chain) {
      auto addr = chain[0];
      try {
        for (auto it = ++chain.begin(); it != chain.end(); ++it) {
          addr = read<uint64_t>(addr);
          addr = addr + *it;
        }
      } catch (memory_exception e) {
        throw memory_exception("Address can't be used right now");
      }
      return addr;
    }

};
