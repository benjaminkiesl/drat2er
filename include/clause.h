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

#ifndef DRAT2ER_CLAUSE_H_
#define DRAT2ER_CLAUSE_H_

#include <vector>
#include <string>
#include <ostream>
#include <initializer_list>

namespace drat2er
{

// Represents a clause (i.e., a disjunction of literals) in a propositional
// CNF formula. You can iterate over the literals of the clause using the
// iterators begin() and end() or their constant counterparts cbegin() and
// cend().
class Clause
{
  friend void swap(Clause& lhs, Clause& rhs);
  friend std::ostream& operator<< (std::ostream& stream, const Clause& clause); 
  
 public:
  Clause();
  Clause(std::initializer_list<int> literals);
  Clause(const Clause& other);
  Clause(Clause&& other);
  Clause& operator=(Clause other);
  
  int GetIndex() const;
  void SetIndex(int index);
  std::vector<int>& GetLiterals();
  const std::vector<int>& GetLiteralsConst() const;
  void SetLiterals(const std::vector<int>& literals);
  void AddLiteral(int literal);
  bool ContainsLiteral(int literal);
  bool IsUnit() const;
  bool IsSubclauseOf(const Clause& other) const;
  int GetMaxVariable() const;
  bool ContainsLiteral(int literal) const;
  std::string ToDimacs() const;
  virtual std::string ToLrat() const;
  auto size() const { return literals_.size(); }
  auto empty() const { return literals_.empty(); }
  auto begin() { return literals_.begin(); }
  auto end() { return literals_.end(); }
  auto cbegin() const { return literals_.cbegin(); }
  auto cend() const { return literals_.cend(); }

 protected:
  std::vector<int> literals_;

 private:
  int index_;
};

inline void SwapLiteralToSecondPosition(const int literal, Clause& clause){
  const int other = clause.GetLiterals()[0]^clause.GetLiterals()[1]^literal;
  clause.GetLiterals()[0] = other; 
  clause.GetLiterals()[1] = literal;
}

} // namespace drat2er

#endif
