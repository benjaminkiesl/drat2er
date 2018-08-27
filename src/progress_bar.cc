#include "progress_bar.h"
#include <string>
#include <iostream>
#include <iomanip>

using std::string;
using std::cout;

namespace drat2er {

ProgressBar::ProgressBar(int overall_number_of_items, int width) 
  : overall_number_of_items_{overall_number_of_items},
    width_{width} {}

ProgressBar::~ProgressBar(){
  cout.flush();
}

void ProgressBar::PrintProgress(int number_of_items_processed){
  PrintProgress(static_cast<double>(number_of_items_processed) 
                / overall_number_of_items_);
}

void ProgressBar::PrintProgress (double percentage) {
    int progress = width_ * percentage;
    string bar(progress, '=');
    string space(width_ - progress, ' ');
    cout << std::fixed << std::setw(4) << std::setprecision(2)  
      << percentage * 100 << "\% [" << bar << space << "]\r" << std::flush;
}

int ProgressBar::GetOverallNumberOfItems(int overall_number_of_items){
  return overall_number_of_items_;
}

void ProgressBar::SetOverallNumberOfItems(int overall_number_of_items){
  overall_number_of_items_ = overall_number_of_items;
}

}
