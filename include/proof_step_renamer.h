#ifndef DRAT2ER_PROOF_STEP_RENAMER_H_
#define DRAT2ER_PROOF_STEP_RENAMER_H_

#include <string>
#include <unordered_map>
#include "proof_transformer.h"

namespace drat2er{

class Deletion;
class Clause;
class RatClause;
class RupClause;

// A ProofStepRenamer takes an input LRAT proof whose instructions are not
// necessarily numbered in ascending order. It then outputs a new LRAT proof
// that is obtained by renaming the instructions of the original proof in
// ascending order.
class ProofStepRenamer : public ProofTransformer {

 public:
  ProofStepRenamer(int index_of_first_proof_step = 1, bool is_verbose=false);

 protected:
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleExtension(const Clause& definition_clause) override;

 private:
  int GetNextClauseIndex();
  int RenameClauseIndex(const int clause_index) const;
  void UpdateRenaming(const int old_clause_index, const int new_clause_index); 
  RupClause RenamePositiveHints(const RupClause& clause);

  std::unordered_map<int, int> clause_renaming_;
  int index_of_next_step_;
};

}

#endif
