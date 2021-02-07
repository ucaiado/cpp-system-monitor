#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, txt, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> txt >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value;
  float f_memtotal = 1.0, f_memfree = 1.0;
  int i_count = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  // NOTE: using answ in https://bit.ly/2MTXWf6
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          f_memtotal = std::stof(value);
          i_count += 1;
        }
        if (key == "MemFree:") {
          f_memfree = std::stof(value);
          i_count += 1;
        }
        if (i_count == 2) {
          break;
        }
      }
    }
  }
  return (f_memtotal - f_memfree) / f_memtotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long system_uptime, idle_uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> system_uptime >> idle_uptime;
  }
  return system_uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto vec_cpu_load = LinuxParser::CpuUtilization();
  // source: https://bit.ly/3cPFsrg
  // NonIdle = user + nice + system + irq + softirq + steal
  // !! from post: guest and guest_nice should be added to NonIdle as well
  vector<int> vec2account{CPUStates::kUser_,     CPUStates::kNice_,
                          CPUStates::kSystem_,   CPUStates::kSoftIRQ_,
                          CPUStates::kSteal_,    CPUStates::kGuest_,
                          CPUStates::kGuestNice_};
  float f_nonidle = 0.0;
  for (int ii : vec2account) {
    f_nonidle += std::stol(vec_cpu_load[ii]);
  }
  return f_nonidle;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto vec_cpu_load = LinuxParser::CpuUtilization();
  // source: https://bit.ly/3cPFsrg
  // Idle = idle + iowait
  vector<int> vec2account{CPUStates::kIdle_, CPUStates::kIOwait_};
  float f_idle = 0.0;
  for (int ii : vec2account) {
    f_idle += std::stol(vec_cpu_load[ii]);
  }
  return f_idle;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> vec_rtn;
  string line, s_cpu_name, s_aux;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    // read just the first line of the file
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> s_cpu_name;
    while (linestream >> s_aux) {
      vec_rtn.push_back(s_aux);
    }
  }
  return vec_rtn;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  int i_processes = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          i_processes = std::stoi(value);
          break;
        }
      }
    }
  }
  return i_processes;

  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
