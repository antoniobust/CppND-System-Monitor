#include "processor.h"

Processor::Processor(const char *id) { cpuId_.assign(id); }

float Processor::Utilization() {
  Processor::MeterRead();
  return this->utilization_;
}

void Processor::MeterRead() {
  Processor::rUsage(LinuxParser::CpuUtilization(this->cpuId_.c_str()));
}

void Processor::rUsage(std::vector<std::string> usage_metrics) {
  float prevIdle = (float)0, prevNonIdle = (float)0, prevTot = (float)0;
  if (usageMetrics_.size() > 0) {
    prevIdle = usageMetrics_.at(LinuxParser::CPUStates::kIdle_) +
               usageMetrics_.at(LinuxParser::CPUStates::kIOwait_);

    prevNonIdle = this->usageMetrics_.at(LinuxParser::CPUStates::kUser_) +
                  this->usageMetrics_.at(LinuxParser::CPUStates::kNice_) +
                  this->usageMetrics_.at(LinuxParser::CPUStates::kSystem_) +
                  this->usageMetrics_.at(LinuxParser::CPUStates::kIRQ_) +
                  this->usageMetrics_.at(LinuxParser::CPUStates::kSoftIRQ_) +
                  this->usageMetrics_.at(LinuxParser::CPUStates::kSteal_);
    prevTot = prevIdle + prevNonIdle;
  }
  this->usageMetrics_.clear();
  for (auto m : usage_metrics) {
    this->usageMetrics_.push_back(std::stof(m));
  }

  float cIdle = this->usageMetrics_.at(LinuxParser::CPUStates::kIdle_) +
                this->usageMetrics_.at(LinuxParser::CPUStates::kIOwait_);

  float cNonIdle = this->usageMetrics_.at(LinuxParser::CPUStates::kUser_) +
                   this->usageMetrics_.at(LinuxParser::CPUStates::kNice_) +
                   this->usageMetrics_.at(LinuxParser::CPUStates::kSystem_) +
                   this->usageMetrics_.at(LinuxParser::CPUStates::kIRQ_) +
                   this->usageMetrics_.at(LinuxParser::CPUStates::kSoftIRQ_) +
                   this->usageMetrics_.at(LinuxParser::CPUStates::kSteal_);
  float cTot = cIdle + cNonIdle;
  float totD = cTot - prevTot;
  float idleD = cIdle - prevIdle;
  utilization_ = (totD - idleD) / totD;
}

std::string Processor::CpuId() { return cpuId_; }