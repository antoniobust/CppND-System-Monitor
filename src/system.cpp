#include "../include/system.h"

#include <unistd.h>

#include <cstddef>
#include <set>

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
std::vector<std::string> cpus = LinuxParser::SystemCpus();
  for (auto c : cpus) {
    cpus_.push_back(Processor(c.c_str()));
  }
}

// TODO: Return the system's CPU
vector<Processor>& System::Cpus() {
  for (auto p : cpus_) {
    LinuxParser::CpuUtilization(p.CpuId());
  }
  return cpus_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }