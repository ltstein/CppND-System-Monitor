#include "processor.h"
#include <vector>
#include <string>
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // static double last_util, last_work, last_idle;
    double utilization{0.0}, work_time{0.0}, idle_time{0.0};
    std::vector<std::string> cpuUtil = LinuxParser::CpuUtilization();
    for (int i{0}; i < cpuUtil.size(); i++){
        int val = std::stoi(cpuUtil[i]);
        if(i == 3){
            idle_time = val;
        }
        else {
            work_time = work_time + val;
        }
    }

    utilization = work_time  /  (work_time + idle_time);

    // utilization = (work_time - last_work) /  ((work_time + idle_time) - (last_work + last_idle));

    // last_util = utilization;
    // last_work = work_time;
    // last_idle = idle_time;
    return utilization;
}