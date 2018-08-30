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
         formula_ {formula},
         max_variable_ {0},
         max_instruction_ {0},
         number_of_instructions_ {0},
         number_of_proper_rat_additions_ {0},
         number_of_rup_additions_ {0},
         number_of_deletions_ {0}
{ }

int ProofStatCollector::GetMaxVariable() const
{
  return max_variable_;
}

int ProofStatCollector::GetMaxInstruction() const
{
  return max_instruction_;
}

int ProofStatCollector::GetNumberOfInstructions() const
{
  return number_of_instructions_;
}

int ProofStatCollector::GetNumberOfProperRatAdditions() const
{
  return number_of_proper_rat_additions_;
}

int ProofStatCollector::GetNumberOfRupAdditions() const
{
  return number_of_rup_additions_;
}

int ProofStatCollector::GetNumberOfDeletions() const
{
  return number_of_deletions_;
}

int ProofStatCollector::GetNumberOfExtensionClauses() const
{
  return number_of_extension_clauses_;
}

void ProofStatCollector::ObserveDeletion(const Deletion& deletion)
{
  max_instruction_ = max(max_instruction_, deletion.GetIndex());
  number_of_instructions_++;
  number_of_deletions_++;
}

void ProofStatCollector::ObserveProperRatAddition(const RatClause& rat)
{
  UpdateInstructionAndMaxVariableStats(rat);
  number_of_proper_rat_additions_++;
}

void ProofStatCollector::ObserveRupAddition(const RupClause& rup)
{
  UpdateInstructionAndMaxVariableStats(rup);
  number_of_rup_additions_++;
}

void ProofStatCollector::ObserveComment(const string& comment_line)
{
  // do nothing
}

void ProofStatCollector::ObserveExtension(const Clause& definition_clause)
{
  UpdateInstructionAndMaxVariableStats(definition_clause);
  number_of_extension_clauses_++;
}

void ProofStatCollector::UpdateInstructionAndMaxVariableStats(
                                                          const Clause& clause)
{
  max_instruction_ = max(max_instruction_, clause.GetIndex());
  max_variable_ = max(max_variable_, clause.GetMaxVariable());
  number_of_instructions_++;
}

} // namespace drat2er
