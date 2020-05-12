#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process() {}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return cpuUtilization_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { upTime_; }

bool Process::operator<(Process const& a) const {
  return this->cpuUtilization_ < a.cpuUtilization_;
}

int Process::Process::Pid(int p) { this->pid_ = p; }
std::string Process::User(const char* u) { this->user_ = std::string(u); }
std::string Process::Command(const char* c) { this->command_ = std::string(c); }
float Process::CpuUtilization(float c_u) { this->cpuUtilization_ = c_u; }
std::string Process::Ram(const char* r) { this->ram_ = std::string(r); }
long int Process::UpTime(long int u_t) { this->upTime_ = u_t; }
