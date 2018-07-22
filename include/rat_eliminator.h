#ifndef DRAT2ER_RAT_ELIMINATOR_H_
#define DRAT2ER_RAT_ELIMINATOR_H_

#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "lrat_parser.h"

namespace drat2er
{

class Formula;
class Clause;
class RupClause;
class RatClause;

class RatEliminator : public LratParserObserver
{
 public:
  RatEliminator(std::string output_file, std::shared_ptr<Formula> formula,
                int max_variable, int max_instruction);
  virtual void HandleProperRatAddition(const RatClause& rat) override;
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleDeletion(const std::vector<int>& clause_indices,
                              int instruction_index) override;
  virtual void HandleComment(const std::string& comment_line) override;

 private:
  int AddDefinitionsForRatClause(const RatClause& clause);
  std::vector<Clause> CorrespondingDefinition(const RatClause& rat, 
                                              const int new_variable);
  void ReplaceOldLiteralByNew(const RatClause& rat, const int new_literal, 
      const int index_of_first_extension, const int index_of_last_extension);
  void DeleteClausesWithOldVariable(const int old_variable);
  int ActualLiteral(int literal);
  void WriteRupToOutput(const RupClause& rup);
  void WriteDefinitionToOutput(const std::vector<Clause>& definition);
  void WriteDeletionToOutput(const std::vector<int>& clause_indices,
                             int instruction_index);

  std::shared_ptr<Formula> formula_;
  int max_variable_;
  int max_instruction_;
  std::ofstream output_stream_;
  std::unordered_map<int,int> old_to_new_literal_;
};

}// namespace

#endif
