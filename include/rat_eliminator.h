#ifndef DRAT2ER_RAT_ELIMINATOR_H_
#define DRAT2ER_RAT_ELIMINATOR_H_

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "proof_transformer.h"

namespace drat2er
{

class Formula;
class Clause;
class RupClause;
class RatClause;

class RatEliminator : public ProofTransformer
{
 public:
  RatEliminator(std::shared_ptr<Formula> formula, 
                int max_variable,
                int max_instruction,
                bool print_progress=false);

  virtual void HandleProperRatAddition(const RatClause& rat) override;
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleDeletion(const Deletion& deletion) override;

  void ReplaceByDefinitionRUPsAndDeletions(const RatClause& rat);
  std::vector<Clause> CorrespondingDefinition(const RatClause& rat, 
                                              const int new_variable);
  Clause FirstDefinitionClause(const RatClause& rat, 
                               const int new_variable) const;
  Clause SecondDefinitionClause(const RatClause& rat, 
                                const int new_variable);
  std::vector<Clause> ThirdBlockOfDefinitionClauses(const RatClause& rat,
                                              const int new_variable);
  template<typename T>
  T RenameLiterals(const T& clause) const;
  RupClause RenameRup(const RupClause& clause) const;
  RatClause RenameRat(const RatClause& clause) const;
  Deletion RenameDeletion(const Deletion& deletion) const;
  int RenameClauseIndex(const int clause_index) const;
  int RenameLiteral(const int literal) const;
  void UpdateLiteralRenaming(const int old_literal, const int new_literal);
  void UpdateClauseRenaming(const int old_index, const int new_index);

 private:
  int AddDefinitionsForRatClause(const RatClause& clause);
  void ReplaceOldPivotByNew(const RatClause& rat, 
                            const std::vector<Clause>& definition);
  void ReplacePositivePivot(const RatClause& rat,
                            const std::vector<Clause>& definition);  
  void ReplaceNegativePivot(const RatClause& rat,
                            const std::vector<Clause>& definition);  
  void DeleteClausesWithOldVariable(const int old_variable,
                                    const std::vector<Clause>& definition);

  void WriteDefinitionToOutput(const std::vector<Clause>& definition);
  void WriteDeletionToOutput(const Deletion& deletion);
  void WriteDeletionToOutput(const std::vector<Clause>& clauses,
                             int instruction_index);
  static void UpdateMapping(std::unordered_map<int,int>& mapping,
                            std::unordered_map<int,int>& inverse_mapping, 
                            const int old_value, const int new_value);

  std::shared_ptr<Formula> formula_;
  int max_variable_;
  int max_instruction_;
  std::unordered_map<int,int> old_to_new_literal_;
  std::unordered_map<int,int> new_to_old_literal_;
  std::unordered_map<int,int> old_to_new_clause_;
  std::unordered_map<int,int> new_to_old_clause_;
};

}// namespace

#endif
