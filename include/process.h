#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  void Pid(int p);
  void User(const char* u);
  void Command(const char* c);
  void CpuUtilization(float c_u);
  void Ram(const char* r);
  void UpTime(long int u_t);

 private:
  int pid_=0;
  long int upTime_=0;
  std::string ram_, user_, command_;
  float cpuUtilization_=(float)0;
};

#endif