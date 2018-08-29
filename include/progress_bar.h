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

#ifndef DRAT2ER_PROGRESS_BAR_H
#define DRAT2ER_PROGRESS_BAR_H

namespace drat2er {

// A ProgressBar can be used to print the current progress of a running
// process to the standard output.
class ProgressBar {
  public:
    // By setting the overall number of items to a given value, you
    // can use the PrintProgress() overload that takes as input the
    // 'number_of_items_processed'. If, for instance, the overall number
    // of items is set to 10, and then PrintProgress() is called with 4,
    // then the progress bar displays a progress of 40%.
    // The parameter 'width' indicates the width (in number of symbols) of
    // the progress bar as displayed in the standard output.
    ProgressBar(int overall_number_of_items=1, int width=22);
    ~ProgressBar();
    int GetOverallNumberOfItems(int overall_number_of_items);
    void SetOverallNumberOfItems(int overall_number_of_items);
    void PrintProgress(int number_of_items_processed);
    void PrintProgress(double percentage);
  private:
    int overall_number_of_items_;
    int width_;
};

} // namespace drat2er

#endif
