#ifndef DRAT2ER_RAT_ELIMINATOR_H_
#define DRAT2ER_RAT_ELIMINATOR_H_

#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "lrat_parser.h"
#include "progress_bar.h"

namespace drat2er
{

class Formula;
class Clause;
class RupClause;
class RatClause;
class ProgressBar;

class RatEliminator : public LratParserObserver
{
 public:
  RatEliminator(std::string output_file, std::shared_ptr<Formula> formula,
                int max_variable, int max_instruction,
                int number_of_proper_rats_overall = 0);
  virtual void HandleProperRatAddition(const RatClause& rat) override;
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleDeletion(const Deletion& deletion) override;
  virtual void HandleComment(const std::string& comment_line) override;

  std::vector<Clause> CorrespondingDefinition(const RatClause& rat, 
                                              const int new_variable);
  Clause FirstDefinitionClause(const RatClause& rat, 
                               const int new_variable) const;
  Clause SecondDefinitionClause(const RatClause& rat, 
                                const int new_variable);
  std::vector<Clause> ThirdBlockOfDefinitionClauses(const RatClause& rat,
                                              const int new_variable);
  template<typename T>
  T RenameClause(const T& clause) const;
  int RenameLiteral(const int literal) const;
  void UpdateRenaming(const int old_literal, const int new_literal);

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

  void WriteRupToOutput(const RupClause& rup);
  void WriteDefinitionToOutput(const std::vector<Clause>& definition);
  void WriteDeletionToOutput(const Deletion& deletion);
  void WriteDeletionToOutput(const std::vector<Clause>& clauses,
                             int instruction_index);
  static void PrintProgress(double percentage);

  std::shared_ptr<Formula> formula_;
  int max_variable_;
  int max_instruction_;
  int number_of_proper_rats_;
  int number_of_proper_rats_overall_;
  std::ofstream output_stream_;
  std::unordered_map<int,int> old_to_new_literal_;
  std::unordered_map<int,int> new_to_old_literal_;
  ProgressBar progress_bar_;
};

template<typename T>
T RatEliminator::RenameClause(const T& clause) const {
  T renamed_clause(clause);
  renamed_clause.SetLiterals(std::vector<int>{});
  for(auto literal : clause){
    renamed_clause.AddLiteral(RenameLiteral(literal));
  }
  return renamed_clause;
}

}// namespace

#endif
