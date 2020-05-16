#include "process.h"

#include <unistd.h>

#include <cctype>
#include <chrono>
#include <sstream>
#include <thread>
#include <vector>

#include "system.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  this->Pid_(pid);
  this->UpTime_(LinuxParser::UpTime(pid));
  this->CpuUtilization_();
  this->User_(LinuxParser::User(pid).c_str());
  this->Command_(LinuxParser::Command(pid).c_str());
  this->Ram_(LinuxParser::Ram(pid).c_str());
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return cpuUtilization_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return upTime_; }

bool Process::operator<(Process const& a) const {
  return a.cpuUtilization_ < this->cpuUtilization_;
}

void Process::Pid_(int p) { pid_ = p; }
void Process::User_(const char* u) { user_ = std::string(u); }
void Process::Command_(const char* c) { command_ = std::string(c); }
void Process::CpuUtilization_() {
  long proc_t_time = LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK);
  cpuUtilization_ = (float)proc_t_time / this->upTime_;
}
void Process::Ram_(const char* r) {
  std::string ram(r);
  ram_ = std::to_string(std::stoi(ram) / 1024);
}
void Process::UpTime_(long int u_t) {
  if (!System::IsModernLinux()) {
    upTime_ = (LinuxParser::Jiffies() / sysconf(_SC_CLK_TCK)) -
              u_t / sysconf(_SC_CLK_TCK);
  } else {
    upTime_ = LinuxParser::UpTime() - u_t / sysconf(_SC_CLK_TCK);
  }
}
