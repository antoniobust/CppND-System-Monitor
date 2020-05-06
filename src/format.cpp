#include "format.h"

#include <cstdlib>
#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  int hours, minutes;
  minutes = seconds / 60;
  hours = minutes / 60;
  return std::to_string(hours) + "h " + std::to_string(minutes % 60) + "m " +
         std::to_string(int(seconds % 60)) + "s ";
}