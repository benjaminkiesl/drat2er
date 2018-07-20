#ifndef DRAT2ER_RAT_ELIMINATOR_H_
#define DRAT2ER_RAT_ELIMINATOR_H_

#include <string>
#include <memory>
#include <vector>

namespace drat2er
{

class Formula;
class Clause;
class RatClause;

std::unique_ptr<RatClause> ParseRat(const std::string& proof_line);

bool IsProperRatAddition(const std::string& proof_line);

std::vector<std::unique_ptr<Clause>> GetCorrespondingDefinition(
    const RatClause& rat, const int new_variable);

class RatEliminator
{
 public:
  void Apply(Formula& formula, const std::string& input_proof_file,
             const std::string& output_proof_file);
 private:
  void AddDefinitionsForRatClause(const RatClause& clause, Formula& formula);
  void ReplaceOldLiteralByNew(const int old_literal, const int new_literal);
  void DeleteClausesWithOldLiteral(const int old_literal);
  void RenameLiteralInRemainingProof(const int old_literal,
                                     const int new_literal);


};

}// namespace

#endif
