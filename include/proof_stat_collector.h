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
  virtual void HandleDeletion(const Deletion& deletion) override;
  virtual void HandleProperRatAddition(const RatClause& rat) override;
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleComment(const std::string& comment_line) override;

 private:
  std::shared_ptr<Formula> formula_;
  int max_variable_;
  int max_instruction_;
};

}// namespace

#endif
