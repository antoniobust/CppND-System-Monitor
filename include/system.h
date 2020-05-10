#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"


class System {
 public:
  System();
  std::vector<Processor>& Cpus();     // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();
  long UpTime();                  // TODO: See src/system.cpp
  int TotalProcesses();           // TODO: See src/system.cpp
  int RunningProcesses();         // TODO: See src/system.cpp
  std::string Kernel();           // TODO: See src/system.cpp
  std::string OperatingSystem();  // TODO: See src/system.cpp

 private:
  std::vector<Processor> cpus_ = {};
  std::vector<Process> processes_ = {};
};

#endif