#include "processor.h"

Processor::Processor(const char *id){
    cpuId_.assign(id);
}
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { return utilization_; }

std::string Processor::CpuId() { return cpuId_;}