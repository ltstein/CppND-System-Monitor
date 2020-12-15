#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "system.h"

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, value, unit;
  string line;
  float total{0.0}, free{0.0};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line) && free < 1) {
      std::istringstream linestream(line);
      while ((linestream >> key >> value >> unit) && free < 1) {
        if (key == "MemTotal:") {
          total = std::stof(value);
        }
        if (key == "MemFree:") {
          free = std::stof(value);
        }
      }
    }
  }

  return (total - free) / total;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return std::stol(uptime);
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
  }
  return {user, nice,    system, idle,  iowait,
          irq,  softirq, steal,  guest, guest_nice};
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key, processes;
  string line;
  int value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line) && !value) {
      std::istringstream linestream(line);
      while ((linestream >> key >> processes) && !value) {
        if (key == "processes") {
          value = std::stoi(processes);
        }
      }
    }
  }

  return value;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key, processes_running;
  string line;
  int value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line) && !value) {
      std::istringstream linestream(line);
      while ((linestream >> key >> processes_running) && !value) {
        if (key == "procs_running") {
          value = std::stoi(processes_running);
        }
      }
    }
  }
  return value;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return line;
}
// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string key, mem_used;
  string line;
  int value{0};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line) && !value) {
      std::istringstream linestream(line);
      while ((linestream >> key >> mem_used) && !value) {
        if (key == "VmSize:") {
          value = std::stoi(mem_used) / 1000;
        }
      }
    }
  }
  return std::to_string(value);
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string key, value;
  string line;
  string uid{""};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line) && !uid.size()) {
      std::istringstream linestream(line);
      while ((linestream >> key >> value) && !uid.size()) {
        if (key == "Uid:") {
          uid = value;
        }
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string user, x, uid;
  string line;
  string value{""};
  string uid_ref = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line) && !value.size()) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while ((linestream >> user >> x >> uid) && !value.size()) {
        if (uid == uid_ref) {
          value = user;
        }
      }
    }
  }
  return value;
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, value;
  long int uptime{0};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line) && !uptime) {
      std::istringstream linestream(line);
      int i = 0;
      while ((linestream >> value) && !uptime) {
        if (i == 21) {
          uptime = std::stol(value);
        }
        i++;
      }
    }
  }
  return UpTime() - (uptime / sysconf(_SC_CLK_TCK));
}

//Calculate CPU utilization for a process based on 
//https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float LinuxParser::ProcessCPUUtil(int pid) {
  string line, value;
  float user{0.0}, kernel{0.0}, childrenU{0.0}, childrenK{0.0};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line) && !childrenK) {
      std::istringstream linestream(line);
      int i = 0;
      while ((linestream >> value) && !childrenK) {
        if (i == 14) {
          user = std::stof(value);
        }
        if (i == 15) {
          kernel = std::stof(value);
        }
        if (i == 16) {
          childrenU = std::stof(value);
        }
        if (i == 17) {
          childrenK = std::stof(value);
        }
        i++;
      }
    }
  }
  double total_time = user + kernel + childrenU + childrenK;
  return (total_time / sysconf(_SC_CLK_TCK)) / UpTime(pid);
}