#include "format.h"
#include <cmath>
#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hours_ = seconds / pow(60, 2);
  int minutes_ = (seconds - hours_ * pow(60, 2)) / 60;
  int seconds_ = (seconds - hours_ * pow(60, 2) - minutes_ * 60);
  // string str_output;
  char str_output[25];
  sprintf(str_output, "%02d:%02d:%02d", hours_, minutes_, seconds_);

  // return std::to_string(seconds);
  return str_output;
}
