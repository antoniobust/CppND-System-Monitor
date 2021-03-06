#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

#include "system.h"

using std::stof;
using std::string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (!filestream.is_open()) {
    return std::string("N/A");
  }
  while (std::getline(filestream, line)) {
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), '=', ' ');
    std::replace(line.begin(), line.end(), '"', ' ');
    std::istringstream linestream(line);
    std::istream_iterator<string> beginning(linestream), end;
    std::vector<string> values(beginning, end);
    if (values[0] == "PRETTY_NAME") {
      std::replace(values[1].begin(), values[1].end(), '_', ' ');
      return values[1];
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
    return 0;
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

long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (!fs.is_open()) {
    return -1;
  }
  string line;
  string utime, stime, cutime, cstime;

  std::getline(fs, line);
  std::istringstream bufLine(line);
  std::istream_iterator<string> beginning(bufLine), end;
  std::vector<string> values(beginning, end);
  return std::stol(values[13]) + std::stol(values[14]) + std::stol(values[15]) +
         std::stol(values[16]);
}

long LinuxParser::ActiveJiffies() {
  std::ifstream fs(kProcDirectory + kStatFilename);
  if (!fs.is_open()) {
    return 0;
  }
  string line;
  std::getline(fs, line);
  std::istringstream s_stream(line);
  std::istringstream bufLine(line);
  std::istream_iterator<string> beginning(bufLine), end;
  std::vector<string> values(beginning, end);
  return std::stol(values[1]) + std::stol(values[2]) + std::stol(values[3]) +
         std::stol(values[4]) + std::stol(values[5]) + std::stol(values[5]) +
         std::stol(values[7]) + std::stol(values[8]) + std::stol(values[9]) +
         std::stol(values[10]);
}

long LinuxParser::IdleJiffies() {
  std::ifstream fs(kProcDirectory + kStatFilename);
  if (!fs.is_open()) {
    return 0;
  }
  string line;
  std::getline(fs, line);
  std::istringstream bufLine(line);
  std::istream_iterator<string> beginning(bufLine), end;
  std::vector<string> values(beginning, end);

  return std::stol(values[4]) + std::stol(values[5]);
}

vector<string> LinuxParser::SystemCpus() {
  std::ifstream fs(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (!fs.is_open()) {
    return {};
  }
  std::string line, label;
  std::istringstream s_stream;
  std::vector<std::string> cpus;
  while (getline(fs, line)) {
    s_stream.clear();
    s_stream.str(line);
    s_stream >> label;
    if (label.find("cpu", 0) == string::npos) {
      break;
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
  std::istringstream s_stream(line);
  // get only the command, ignore the flags (long strings mess up with display)
  s_stream >> cmd;
  return cmd;
}

string LinuxParser::Ram(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (!fs.is_open()) {
    return string("0");
  }
  string value("0"), label, line;
  std::istringstream s_stream;
  while (getline(fs, line)) {
    s_stream.clear();
    s_stream.str(line);
    s_stream >> label >> value;
    if (label == "VmSize:") {
      break;
    }
  }
  return value;
}

string LinuxParser::Uid(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (!fs.is_open()) {
    return string("N/A");
  }
  string line(""), uid("");
  std::istringstream s_stream;
  while (std::getline(fs, line)) {
    if (line.find("Uid", 0) != string::npos) {
      s_stream.clear();
      s_stream.str(line);
      s_stream >> uid >> uid;
      break;
    }
  }
  return uid;
}

string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  std::ifstream fs(kPasswordPath);
  if (!fs.is_open()) {
    return string("N/A");
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
      break;
    }
  }
  return user;
}

long LinuxParser::UpTime(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (!fs.is_open()) {
    return 0;
  }
  std::string line, major, minor;
  std::getline(fs, line);
  std::istringstream bufLine(line);
  std::istream_iterator<string> beginning(bufLine), end;
  std::vector<string> values(beginning, end);

  return std::stol(values[21]);
}

bool LinuxParser::IsProcess(std::filesystem::directory_entry d) {
  std::string name = d.path().filename();
  return name.find_first_not_of("0123456789") > name.length();
}