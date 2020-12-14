#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { pid_ = pid; }

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
    cpu_util_ = LinuxParser::ProcessCPUUtil(pid_);
    return cpu_util_; }

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { 
    ram_ = LinuxParser::Ram(pid_); 
    return ram_;
    }

// DONE: Return the user (name) that generated this process
string Process::User() { 
    user_ = LinuxParser::User(pid_);
    return  user_;
    }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
    uptime_ = LinuxParser::UpTime(pid_);
    return uptime_;
     }

//DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a ) const {
  return cpu_util_ < a.cpu_util_;
}
