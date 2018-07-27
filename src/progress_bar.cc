#include "progress_bar.h"
#include <string>
#include <iostream>
#include <iomanip>

using std::string;
using std::cout;

namespace drat2er {

ProgressBar::ProgressBar(int width) : width_{width} {}

void ProgressBar::PrintProgress (double percentage) {
    int progress = width_ * percentage;
    string bar(progress, '=');
    string space(width_ - progress, ' ');
    cout << std::fixed << std::setw(4) << std::setprecision(2)  
      << percentage * 100 << "\% [" << bar << space << "]\r" << std::flush;
}

}
