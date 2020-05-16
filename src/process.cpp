#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
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
  return this->cpuUtilization_ < a.cpuUtilization_;
}

void Process::Pid_(int p) { pid_ = p; }
void Process::User_(const char* u) { user_ = std::string(u); }
void Process::Command_(const char* c) { command_ = std::string(c); }
void Process::CpuUtilization_() {
  if (!System::IsModernLinux()) {
    long c_Jiffies = LinuxParser::ActiveJiffies(this->pid_);
    long c_totJiffies =
        LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
    cpuUtilization_ =
        (float)(c_Jiffies - this->jiffies) / (c_totJiffies - this->totJiffies);
    this->totJiffies = c_totJiffies;
    this->jiffies = c_Jiffies;
  } else {
    long total_time = LinuxParser::ActiveJiffies(pid_);
    long seconds = this->Process::UpTime();
    long total_time_seconds = total_time / sysconf(_SC_CLK_TCK);
    cpuUtilization_ = (float)total_time_seconds / seconds;
    // long total_time = LinuxParser::ActiveJiffies(pid_);
    // long c_seconds = UpTime();
    // long c_total_time_seconds = total_time / sysconf(_SC_CLK_TCK);
    // long delta_total = c_total_time_seconds - total_time_seconds_;
    // long delta_seconds = c_seconds - seconds_;
    // total_time_seconds_ = c_total_time_seconds;
    // seconds_ = c_seconds;
    // cpuUtilization_ = (float)delta_total / delta_seconds;
  }
}
void Process::Ram_(const char* r) {
  std::string ram(r);
  ram_ = std::to_string(std::stoi(ram) / 1024);
}
void Process::UpTime_(long int u_t) { upTime_ = u_t; }
