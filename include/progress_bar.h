#ifndef DRAT2ER_PROGRESS_BAR_H
#define DRAT2ER_PROGRESS_BAR_H

namespace drat2er {

class ProgressBar {
  public:
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
