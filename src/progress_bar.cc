// MIT License
//
// Copyright (c) 2018 Benjamin Kiesl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "progress_bar.h"
#include <string>
#include <iostream>
#include <iomanip>

using std::string;
using std::cout;

namespace drat2er
{

ProgressBar::ProgressBar(int overall_number_of_items, int width)
  : overall_number_of_items_ {overall_number_of_items},
    width_ {width} {}

ProgressBar::~ProgressBar()
{
  cout.flush();
}

int ProgressBar::GetOverallNumberOfItems() const
{
  return overall_number_of_items_;
}

void ProgressBar::SetOverallNumberOfItems(int overall_number_of_items)
{
  overall_number_of_items_ = overall_number_of_items;
}

void ProgressBar::PrintProgress(int number_of_items_processed) const
{
  PrintProgress(static_cast<double>(number_of_items_processed)
                / overall_number_of_items_);
}

void ProgressBar::PrintProgress (double percentage) const
{
  int progress = width_ * percentage;
  string bar(progress, '=');
  string space(width_ - progress, ' ');
  cout << std::fixed << std::setw(4) << std::setprecision(2)
       << percentage * 100 << "\% [" << bar << space << "]\r" << std::flush;
}

} // namespace drat2er
