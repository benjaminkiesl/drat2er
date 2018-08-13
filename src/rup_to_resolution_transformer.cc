#include "rup_to_resolution_transformer.h"
#include <string>
#include <memory>
#include <cassert>
#include "formula.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "deletion.h"

using std::string;
using std::endl;

namespace drat2er {

RupToResolutionTransformer::RupToResolutionTransformer(
                             std::shared_ptr<Formula> formula,
                             bool is_output_drat,
                             bool print_progress) :
                                ProofTransformer(
                                    "Transforming RUPs to resolution chains.", 
                                    print_progress),
                                formula_(formula), 
                                found_empty_clause_{false} {}

void RupToResolutionTransformer::HandleExtension(const Clause& extension){
  if(!found_empty_clause_){
    formula_->AddClause(extension);
    if(is_output_lrat_){
      OutputStream() << extension.GetIndex() << " e " << 
        extension.ToDimacs() << endl;
    } else {
      OutputStream() << extension.ToDimacs() << endl;
    }
  }
}

void RupToResolutionTransformer::HandleRupAddition(const RupClause& rup){
  if(!found_empty_clause_){
    Formula sub_formula;
    for(auto clause_index : rup.GetPositiveHints()){
      assert(formula_->GetClause(clause_index) != nullptr);
      sub_formula.AddClause(*formula_->GetClause(clause_index));
    }
    auto subsuming_rup = sub_formula.DeriveSubsumingClause(rup);
    assert(subsuming_rup != nullptr);
    formula_->AddClause(*subsuming_rup);
    WriteClauseToOutput(*subsuming_rup);
    if(subsuming_rup->empty()){
      found_empty_clause_ = true;
    }
  }
}

} // namespace
