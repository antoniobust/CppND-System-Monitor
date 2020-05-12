#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <vector>

using std::string;
using std::to_string;
using std::vector;


int Process::Pid() {
  return pid_;
}

float Process::CpuUtilization() { return cpuUtilization_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return upTime_; }

bool Process::operator<(Process const& a) const {
  return this->cpuUtilization_ < a.cpuUtilization_;
}

void Process::Pid(int p) { pid_ = p; }
void Process::User(const char* u) { user_ = std::string(u); }
void Process::Command(const char* c) { command_ = std::string(c); }
void Process::CpuUtilization(float c_u) { cpuUtilization_ = c_u; }
void Process::Ram(const char* r) { ram_ = std::string(r); }
void Process::UpTime(long int u_t) { upTime_ = u_t; }
