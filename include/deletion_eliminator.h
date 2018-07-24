#ifndef DRAT2ER_DELETION_ELIMINATOR_H_
#define DRAT2ER_DELETION_ELIMINATOR_H_

#include <string>
#include <vector>
#include <fstream>
#include "lrat_parser.h"

namespace drat2er
{

class DeletionEliminator : public LratParserObserver
{
 public:
  DeletionEliminator(std::string output_file);
  virtual void HandleDeletion(const std::vector<int>& clause_indices,
                              int instruction_index) override;
  virtual void HandleProperRatAddition(const RatClause& rat) override;
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleComment(const std::string& comment_line) override;
 private:
  std::ofstream output_stream_;
};

}// namespace

#endif
