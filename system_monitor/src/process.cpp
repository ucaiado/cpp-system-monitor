#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }
void Process::Pid(int pid) { pid_ = pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return process_cpu_; }
void Process::CpuUtilization(float process_cpu) { process_cpu_ = process_cpu; }

// Return the command that generated this process
string Process::Command() { return cmd_; }
void Process::Command(string cmd) { cmd_ = cmd; }

// Return this process's memory utilization
string Process::Ram() { return process_ram_; }
void Process::Ram(string process_ram) { process_ram_ = process_ram; }

// Return the user (name) that generated this process
string Process::User() { return user_; }
void Process::User(string user) { user_ = user; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return process_uptime_; }
void Process::UpTime(long int process_uptime) {
  process_uptime_ = process_uptime;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return a.CpuUtilization() < process_cpu_;
}
