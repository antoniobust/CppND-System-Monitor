#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>
#include <linux_parser.h> 

class Processor {
 public:
  Processor();
  Processor(const char *id);
  float Utilization();
  std::string CpuId();

 private:
  void MeterRead();
  void rUsage(std::vector<std::string> usage_metrics);
  std::string cpuId_;
  float utilization_ = 0.0;
  std::vector<float> usageMetrics_;
};

#endif