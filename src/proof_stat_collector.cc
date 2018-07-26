#include "proof_stat_collector.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "formula.h"
#include "clause.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "deletion.h"
#include "lrat_parser.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::max;
using std::endl;
using std::cout;

namespace drat2er
{

ProofStatCollector::ProofStatCollector(shared_ptr<Formula> formula) : 
                                        formula_{formula},
                                        max_variable_{0},
                                        max_instruction_{0}
                                        { }

int ProofStatCollector::GetMaxVariable() {
  return max_variable_;
}

int ProofStatCollector::GetMaxInstruction() {
  return max_instruction_;
}

void ProofStatCollector::HandleDeletion(const Deletion& deletion){
  max_instruction_ = max(max_instruction_, deletion.GetIndex());
} 

void ProofStatCollector::HandleProperRatAddition(const RatClause& rat){
  max_variable_ = max(max_variable_, rat.GetMaxVariable());
  max_instruction_ = max(max_instruction_, rat.GetIndex());
}

void ProofStatCollector::HandleRupAddition(const RupClause& rup){
  max_variable_ = max(max_variable_, rup.GetMaxVariable());
  max_instruction_ = max(max_instruction_, rup.GetIndex());
}

void ProofStatCollector::HandleComment(const string& comment_line){
  // do nothing
}

} // namespace
