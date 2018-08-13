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

class ProofStepRenamer : public ProofTransformer {

 public:
  ProofStepRenamer(int index_of_first_proof_step = 1);

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
