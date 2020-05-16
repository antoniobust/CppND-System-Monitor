#include "../include/system.h"

#include <unistd.h>

#include <cstddef>
#include <set>

#include "process.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  std::vector<std::string> cpus = LinuxParser::SystemCpus();
  for (auto c : cpus) {
    cpus_.push_back(Processor(c.c_str()));
  }
  // Assign non changing properties here
  kernel.assign(Kernel_());
  OS.assign(OperatingSystem_());
}

bool System::IsModernLinux() {
  string minor, major;
  std::istringstream s_stream(LinuxParser::Kernel());
  std::getline(s_stream, major, '.');
  std::getline(s_stream, minor, '.');

  if (std::stoi(major) == 2 && std::stoi(minor) == 6) {
    return false;
  } else {
    return true;
  }
}

vector<Processor>& System::Cpus() {
  for (auto p : cpus_) {
    LinuxParser::CpuUtilization(p.CpuId());
  }
  return cpus_;
}

vector<Process>& System::Processes() {
  std::vector<int> c_pids = LinuxParser::Pids();
  processes_.clear();

  for (int& p : c_pids) {
    Process proc = Process(p);
    processes_.push_back(proc);
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

std::string System::Kernel_() { return LinuxParser::Kernel(); }

std::string System::Kernel() { return this->kernel; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return this->OS; }

std::string System::OperatingSystem_() {
  return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }