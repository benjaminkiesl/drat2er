// MIT License
//
// Copyright (c) 2018 Benjamin Kiesl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
// IN THE SOFTWARE.

#ifndef DRAT2ER_PROOF_STAT_COLLECTOR_H_
#define DRAT2ER_PROOF_STAT_COLLECTOR_H_

#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include "lrat_parser.h"

namespace drat2er
{

class Formula;
class Clause;
class RatClause;

// A ProofStatCollector is an observer of an LratParser. It just collects
// some information about a parsed LRAT proof, such as the number of
// instructions, number of deletions, etc.
class ProofStatCollector : public LratParserObserver
{
 public:
  ProofStatCollector(std::shared_ptr<Formula> formula);
  int GetMaxVariable() const;
  int GetMaxInstruction() const;
  int GetNumberOfInstructions() const;
  int GetNumberOfProperRatAdditions() const;
  int GetNumberOfRupAdditions() const;
  int GetNumberOfDeletions() const;
  int GetNumberOfExtensionClauses() const;
  virtual void ObserveDeletion(const Deletion& deletion) override;
  virtual void ObserveProperRatAddition(const RatClause& rat) override;
  virtual void ObserveRupAddition(const RupClause& rup) override;
  virtual void ObserveComment(const std::string& comment_line) override;
  virtual void ObserveExtension(const Clause& definition_clause) override;

 private:
  void UpdateInstructionAndMaxVariableStats(const Clause& clause);

  std::shared_ptr<Formula> formula_;
  int max_variable_;
  int max_instruction_;
  int number_of_instructions_;
  int number_of_proper_rat_additions_;
  int number_of_rup_additions_;
  int number_of_deletions_;
  int number_of_extension_clauses_;
};

} // namespace drat2er

#endif
