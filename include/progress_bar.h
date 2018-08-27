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

}

#endif
