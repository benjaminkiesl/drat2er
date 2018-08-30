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

#include "instruction_serialization.h"
#include <string>
#include <sstream>
#include "clause.h"
#include "rup_clause.h"
#include "rat_clause.h"
#include "deletion.h"

using std::string;
using std::stringstream;

namespace drat2er {

string ToDIMACS(const Clause& clause)
{
  stringstream ss;
  for(auto literal : clause.GetLiteralsConst()) {
    ss << literal << ' ';
  }
  ss << '0';
  return ss.str();
}

string ToLRAT(const Clause& clause){
  stringstream ss;
  ss << clause.GetIndex() << ' ' << ToDIMACS(clause) << " 0";
  return ss.str();
}

string ToLRATExtension(const Clause& clause)
{
  stringstream ss;
  ss << clause.GetIndex() << " e " << ToDIMACS(clause);
  return ss.str();
}

string ToLRAT(const RupClause& rup)
{
  stringstream ss;
  ss << rup.GetIndex() << ' ' << ToDIMACS(rup) << ' ';
  for(auto hint : rup.GetPositiveHints()) {
    ss << hint << ' ';
  }
  ss << '0';
  return ss.str();
}

string ToLRAT(const RatClause& rat)
{
  stringstream ss;
  ss << rat.GetIndex() << ' ' << ToDIMACS(rat) << ' ';
  for(auto hint : rat.GetPositiveHints()) {
    ss << hint << ' ';
  }
  for(auto hint : rat.GetNegativeHints()) {
    ss << -hint.first << ' ';
    for(auto sub_hint : hint.second) {
      ss << sub_hint << ' ';
    }
  }
  ss << '0';
  return ss.str();
}

string ToLRAT(const Deletion& deletion)
{
  stringstream ss;
  ss << deletion.GetIndex() << " d ";
  for(auto index : deletion.GetClauseIndices()) {
    ss << index << " ";
  }
  ss << "0";
  return ss.str();
}

string ToTRACECHECKExtension(const Clause& definition_clause)
{
  return ToLRAT(definition_clause);
}

string ToTRACECHECK(const RupClause& rup)
{
  return ToLRAT(rup);
}

string ToDRAT(const Clause& clause)
{
  return ToDIMACS(clause);
}

} // namespace drat2er
