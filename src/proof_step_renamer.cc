#include "proof_step_renamer.h"
#include "clause.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "lrat_parser.h"
#include "deletion.h"

using std::string;
using std::endl;

namespace drat2er
{

ProofStepRenamer::ProofStepRenamer(int index_of_first_proof_step, 
                                   bool print_progress) 
                           : ProofTransformer(
                               "Renaming proof steps incrementally.", 
                               print_progress),
                             clause_renaming_{},
                             index_of_next_step_{index_of_first_proof_step}{}

void ProofStepRenamer::HandleExtension(const Clause& extension){
  Clause renamed_extension(extension);
  renamed_extension.SetIndex(GetNextClauseIndex());
  UpdateRenaming(extension.GetIndex(), renamed_extension.GetIndex());
  WriteClauseToOutput(renamed_extension);
}

void ProofStepRenamer::HandleRupAddition(const RupClause& rup){
  auto renamed_rup = RenamePositiveHints(rup);
  int old_index = rup.GetIndex();
  renamed_rup.SetIndex(GetNextClauseIndex());
  UpdateRenaming(old_index, renamed_rup.GetIndex());
  WriteClauseToOutput(renamed_rup);
}

int ProofStepRenamer::GetNextClauseIndex(){
  return index_of_next_step_++;
}

int ProofStepRenamer::RenameClauseIndex(const int clause_index) const {
  if(clause_renaming_.find(clause_index) != clause_renaming_.end()){
    return clause_renaming_.at(clause_index);
  }
  return clause_index;
}

void ProofStepRenamer::UpdateRenaming(const int old_clause_index, 
                                      const int new_clause_index){
  clause_renaming_[old_clause_index] = new_clause_index;
}

RupClause ProofStepRenamer::RenamePositiveHints(const RupClause& clause){
  RupClause renamed_clause(clause);
  renamed_clause.ClearPositiveHints();
  for(auto hint : clause.GetPositiveHints()){
    renamed_clause.AddPositiveHint(RenameClauseIndex(hint));
  }
  return renamed_clause;
}

} // namespace
