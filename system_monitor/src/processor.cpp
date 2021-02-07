#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  // source: https://bit.ly/3cPFsrg
  // NonIdle = user + nice + system + irq + softirq + steal
  float f_nonidle = LinuxParser::ActiveJiffies();
  // Idle = idle + iowait
  float f_idle = LinuxParser::IdleJiffies();
  // Total = Idle + NonIdle
  // CPU_Percentage = (Total - idled)/totald
  if ((f_nonidle + f_idle) == 0) {
    return 0.0;
  }
  return (f_nonidle / (f_nonidle + f_idle));
}
