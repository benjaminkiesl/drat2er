#ifndef DRAT2ER_PROOF_STAT_COLLECTOR_H_
#define DRAT2ER_PROOF_STAT_COLLECTOR_H_

#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include "lrat_parser.h"

namespace drat2er
{

class Formula;
class Clause;
class RatClause;

class ProofStatCollector : public LratParserObserver
{
 public:
  ProofStatCollector(std::shared_ptr<Formula> formula);
  int GetMaxVariable();
  int GetMaxInstruction();
  int GetNumberOfInstructions();
  int GetNumberOfProperRatAdditions();
  int GetNumberOfRupAdditions();
  int GetNumberOfDeletions();
  virtual void ObserveDeletion(const Deletion& deletion) override;
  virtual void ObserveProperRatAddition(const RatClause& rat) override;
  virtual void ObserveRupAddition(const RupClause& rup) override;
  virtual void ObserveComment(const std::string& comment_line) override;

 private:
  std::shared_ptr<Formula> formula_;
  int max_variable_;
  int max_instruction_;
  int number_of_instructions_;
  int number_of_proper_rat_additions_;
  int number_of_rup_additions_;
  int number_of_deletions_;
};

}// namespace

#endif
