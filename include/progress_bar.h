#ifndef DRAT2ER_PROGRESS_BAR_H
#define DRAT2ER_PROGRESS_BAR_H

namespace drat2er {

class ProgressBar {
  public:
    ProgressBar(int width=22);
    void PrintProgress(double percentage);
  private:
    int width_;
};

}

#endif
