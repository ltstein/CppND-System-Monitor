#include <string>
#include <sstream>
#include <ostream>
#include <iomanip>
#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int seconds) { 
    int secperhour{3600}, secpermin{60};
    int hr, min, sec;
    hr = seconds / secperhour;
    min = (seconds % secperhour) / secpermin;
    sec = seconds - (hr * secperhour) - (min * secpermin);

    //Formatting based on leads from https://stackoverflow.com/a/6143927
    string elapsed;
    std::ostringstream conversion;

    conversion << std::setfill('0') << std::setw(2) << hr << ":";
    conversion << std::setfill('0') << std::setw(2) << min << ":";
    conversion << std::setfill('0') << std::setw(2) << sec;
    elapsed = conversion.str();
    return elapsed; 
    }