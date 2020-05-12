#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process();
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  int Pid(int p);
  std::string User(const char* u);
  std::string Command(const char* c);
  float CpuUtilization(float c_u);
  std::string Ram(const char* r);
  long int UpTime(long int u_t);

 private:
  int pid_ = 0;
  long int upTime_ = 0;
  std::string ram_, user_, command_;
  float cpuUtilization_;
};

#endif