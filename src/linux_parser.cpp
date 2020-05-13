#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  std::filesystem::path proc = kProcDirectory;
  for (auto& f : std::filesystem::directory_iterator(proc)) {
    if (f.is_directory() && LinuxParser::IsProcess(f)) {
      pids.push_back(std::stoi(f.path().filename()));
    }
  }
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line, label, tot, free, buffers;
  std::ifstream fs(kProcDirectory + kMeminfoFilename);
  if (!fs.is_open()) {
    return 0.0;
  }
  std::istringstream s_stream;
  std::getline(fs, line);
  s_stream.str(line);
  s_stream >> label >> tot;
  std::getline(fs, line);
  s_stream.str(line);
  s_stream >> label >> free;
  std::getline(fs, line);
  std::getline(fs, line);
  s_stream.str(line);
  s_stream >> label >> buffers;

  return 1.0 - (std::stof(free) / (std::stof(tot) - std::stof(buffers)));
}

long LinuxParser::UpTime() {
  std::ifstream fs(kProcDirectory + kUptimeFilename);
  if (!fs.is_open()) {
    return 0;
  }
  string up_time, line;
  getline(fs, line);
  std::istringstream s_stream(line);
  s_stream >> up_time;
  long int a = std::stol(up_time);
  return a;
}

long LinuxParser::Jiffies() { return sysconf(_SC_CLK_TCK) * UpTime(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (!fs.is_open()) {
    return -1;
  }
  string line;
  string utime, stime, cutime, cstime;

  std::getline(fs, line);
  std::istringstream s_stream(line);
  s_stream.seekg(13);
  s_stream >> utime >> stime >> cutime >> cstime;
  return std::stol(utime) + std::stol(stime) + std::stol(cutime) +
         std::stol(cstime);
}
// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::SystemCpus() {
  std::ifstream fs(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (!fs.is_open()) {
    return {};
  }
  std::string line, label;
  std::istringstream s_stream;
  std::vector<std::string> cpus;
  while (getline(fs, line)) {
    s_stream.str(line);
    s_stream >> label;
    if (label.find("cpu", 0)) {
      return cpus;
    } else {
      cpus.push_back(label);
    }
  }
  return cpus;
}

vector<string> LinuxParser::CpuUtilization(std::string id) {
  std::ifstream fs(kProcDirectory + kStatFilename);
  if (!fs.is_open()) {
    return {};
  }
  std::string line;
  while (std::getline(fs, line)) {
    if (line.find(id) != string::npos) {
      std::istringstream s_stream(line);
      std::vector<string> c_usage;
      std::string val;
      s_stream >> val;  // skip first stream (cpu id);
      while (!s_stream.eof()) {
        s_stream >> val;
        c_usage.push_back(val);
      }
      return c_usage;
    }
  }

  return {};
}

int LinuxParser::TotalProcesses() {
  int tot_p = 0;
  std::filesystem::path proc = kProcDirectory;
  for (auto& f : std::filesystem::directory_iterator(proc)) {
    if (f.is_directory()) {
      if (LinuxParser::IsProcess(f)) {
        tot_p++;
      }
    }
  }
  return tot_p;
}

int LinuxParser::RunningProcesses() {
  std::ifstream fs(kProcDirectory + kStatFilename);
  string line, procs_running("procs_running"), ps = "0";
  if (!fs.is_open()) {
    return 0;
  }
  while (!std::getline(fs, line).eof()) {
    if (line.find(procs_running, 0) != string::npos) {
      std::istringstream s_stream(line);
      s_stream >> ps >> ps;
      break;
    }
  }

  return std::stoi(ps);
}

string LinuxParser::Command(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (!fs.is_open()) {
    return "";
  }
  string line, cmd;
  std::getline(fs, line);
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

string LinuxParser::Uid(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (!fs.is_open()) {
    return string();
  }
  string line, uid;
  std::istringstream s_stream;
  while (std::getline(fs, line)) {
    if (line.find("Uid", 0) != string::npos) {
      s_stream.clear();
      s_stream.str(line);
      s_stream >> uid >> uid;
      return uid;
    }
  }
  return line;
}

string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  std::ifstream fs(kPasswordPath);
  if (!fs.is_open()) {
    return "";
  }
  string line;
  string user, userId, pwd;
  std::istringstream s_stream;
  while (std::getline(fs, line)) {
    s_stream.clear();
    s_stream.str(line);
    std::getline(s_stream, user, ':');
    std::getline(s_stream, pwd, ':');
    std::getline(s_stream, userId, ':');
    if (uid.compare(userId) == 0) {
      return user;
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }

bool LinuxParser::IsProcess(std::filesystem::directory_entry d) {
  string name = d.path().filename();
  return name.find_first_not_of("0123456789") > name.length();
}