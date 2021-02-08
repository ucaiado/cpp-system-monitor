#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  void Pid(int pid);
  void CpuUtilization(float process_cpu);
  void Command(std::string cmd);
  void Ram(std::string process_ram);
  void User(std::string user);
  void UpTime(long int process_uptime);

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string cmd_;
  float process_cpu_;
  std::string process_ram_;
  long int process_uptime_;
};

#endif
