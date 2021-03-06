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

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  // source: https://knowledge.udacity.com/questions/129844
  // sysconf(_SC_CLK_TCK) - > system frequency: the number of ticks per second
  return UpTime() * sysconf(_SC_CLK_TCK);
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  // source: https://bit.ly/3rrLlPe
  // total_time = utime + stime + cutime + cstime
  string line, value;
  float f_jiffies = 0.0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int ii = 0; ii < 17; ii++) {
        linestream >> value;
        if (ii >= 13) {
          f_jiffies = std::stol(value);
        }
      }
    }
  }
  return f_jiffies;
}

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
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  int i_running_processes = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          i_running_processes = std::stoi(value);
          break;
        }
      }
    }
  }
  return i_running_processes;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string s_cmd = "";
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, s_cmd);
    return s_cmd;
  }
  return s_cmd;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value;
  int i_vm_size = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          i_vm_size = std::stoi(value);
          break;
        }
      }
    }
  }
  // convert to MB
  return std::to_string(i_vm_size / 1024);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value, s_uid = "";
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          s_uid = value;
          break;
        }
      }
    }
  }
  return s_uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, s_this_x, s_this_uid, s_this_usrname, s_usrname = "NoUser";
  string s_desired_uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    // NOTE: c++ understands ':'singlequoted as char.... for some reason
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::getline(linestream, s_this_usrname, ':');
      std::getline(linestream, s_this_x, ':');
      std::getline(linestream, s_this_uid, ':');
      if (s_this_uid == s_desired_uid) {
        s_usrname = s_this_usrname;
        break;
      }
    }
  }
  return s_usrname;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, value;
  float f_jiffies = 0.0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int ii = 0; ii < 22; ii++) {
        linestream >> value;
        if (ii == 21) {
          f_jiffies = std::stol(value);
        }
      }
    }
  }
  return (UpTime() - (f_jiffies / sysconf(_SC_CLK_TCK)));
}
