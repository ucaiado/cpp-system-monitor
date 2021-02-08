#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> vec_pids = LinuxParser::Pids();
  // float f_nonidle = LinuxParser::ActiveJiffies();
  // float f_idle = LinuxParser::IdleJiffies();
  // for (int pid : vec_pids) {
  //   Process this_process;
  //   this_process.Pid(pid);
  //   this_process.CpuUtilization(LinuxParser::ActiveJiffies(pid) /
  //                               (f_nonidle + f_idle));
  //   this_process.Command(LinuxParser::Command(pid));
  //   this_process.Ram(LinuxParser::Ram(pid));
  //   processes_.push_back(this_process);
  // }
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
