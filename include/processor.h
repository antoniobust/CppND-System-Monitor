#ifndef PROCESSOR_H
#define PROCESSOR_H

#include<string>

class Processor {
 public:
  Processor(const char *id);
  float Utilization();
  std::string CpuId();

  // TODO: Declare any necessary private members
 private:
    std::string cpuId_;
    float utilization_ = 0.0;
};

#endif