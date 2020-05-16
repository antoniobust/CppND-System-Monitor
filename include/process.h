#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"

class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid_ = 0;
  long totJiffies = 0, jiffies = 0, seconds_ = 0, total_time_seconds_ = 0;
  long int upTime_ = 0;
  std::string ram_, user_, command_;
  float cpuUtilization_ = (float)0;
  void Pid_(int p);
  void User_(const char* u);
  void Command_(const char* c);
  void CpuUtilization_();
  void Ram_(const char* r);
  void UpTime_(long int u_t);
};

#endif