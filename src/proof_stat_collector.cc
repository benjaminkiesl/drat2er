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
                                        max_instruction_{0},
                                        number_of_instructions_{0},
                                        number_of_proper_rat_additions_{0},
                                        number_of_rup_additions_{0},
                                        number_of_deletions_{0}
                                        { }

int ProofStatCollector::GetMaxVariable() {
  return max_variable_;
}

int ProofStatCollector::GetMaxInstruction() {
  return max_instruction_;
}

int ProofStatCollector::GetNumberOfInstructions(){
  return number_of_instructions_;
}

int ProofStatCollector::GetNumberOfProperRatAdditions(){
  return number_of_proper_rat_additions_;
}

int ProofStatCollector::GetNumberOfRupAdditions(){
  return number_of_rup_additions_;
}

int ProofStatCollector::GetNumberOfDeletions(){
  return number_of_deletions_;
}

void ProofStatCollector::ObserveDeletion(const Deletion& deletion){
  max_instruction_ = max(max_instruction_, deletion.GetIndex());
  number_of_deletions_++;
  number_of_instructions_++;
} 

void ProofStatCollector::ObserveProperRatAddition(const RatClause& rat){
  max_variable_ = max(max_variable_, rat.GetMaxVariable());
  max_instruction_ = max(max_instruction_, rat.GetIndex());
  number_of_proper_rat_additions_++;
  number_of_instructions_++;
}

void ProofStatCollector::ObserveRupAddition(const RupClause& rup){
  max_variable_ = max(max_variable_, rup.GetMaxVariable());
  max_instruction_ = max(max_instruction_, rup.GetIndex());
  number_of_rup_additions_++;
  number_of_instructions_++;
}

void ProofStatCollector::ObserveComment(const string& comment_line){
  // do nothing
}

} // namespace
