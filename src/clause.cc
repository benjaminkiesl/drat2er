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

#include "clause.h"

#include <utility>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <initializer_list>

using std::string;
using std::stringstream;
using std::vector;
using std::unordered_set;
using std::cout;
using std::endl;
using std::max;
using std::find;
using std::max_element;
using std::abs;
using std::initializer_list;

namespace drat2er
{

void swap(Clause& lhs, Clause& rhs)
{
  using std::swap;
  swap(lhs.index_, rhs.index_);
  swap(lhs.literals_, rhs.literals_);
}


Clause::Clause() : index_ {-1}, literals_ {} { }

Clause::Clause(initializer_list<int> literals) : Clause() {
  for(auto literal : literals){
    AddLiteral(literal);
  }
}

Clause::Clause(const Clause& other) : index_{other.index_},
                                      literals_(other.literals_){}

Clause::Clause(Clause&& other) : Clause()
{
  swap(*this, other);
}

Clause& Clause::operator=(Clause other)
{
  swap(*this, other);
  return *this;
}

int Clause::GetIndex() const
{
  return index_;
}

void Clause::SetIndex(int index)
{
  index_ = index;
}

vector<int>& Clause::GetLiterals() {
  return literals_;
}

const vector<int>& Clause::GetLiteralsConst() const {
  return literals_;
}

void Clause::SetLiterals(const vector<int>& literals)
{
  literals_.clear();
  for(auto literal : literals) {
    AddLiteral(literal);
  }
}

void Clause::AddLiteral(const int literal)
{
  literals_.emplace_back(literal);
}

bool Clause::ContainsLiteral(int literal){
  return find(literals_.begin(), literals_.end(), literal) !=
    literals_.end();
}

bool Clause::IsUnit() const {
  return literals_.size() == 1;
}

bool Clause::IsSubclauseOf(const Clause& other) const {
  unordered_set<int> literals_of_clause(cbegin(), cend());
  unordered_set<int> literals_of_other(other.cbegin(), other.cend());
  for(auto literal : literals_of_clause){
    if(literals_of_other.find(literal) == literals_of_other.end()){
      return false;
    }
  }
  return true;
}

int Clause::GetMaxVariable() const
{
  int max_variable = 0;
  for(auto literal : literals_){
    max_variable = max(max_variable, abs(literal));
  }
  return max_variable;
}

bool Clause::ContainsLiteral(const int literal) const
{
  return find(literals_.begin(), literals_.end(), literal) != literals_.end();
}

string Clause::ToDimacs() const
{
  stringstream ss;
  for(auto literal : literals_) {
    ss << literal << ' ';
  }
  ss << '0';
  return ss.str();
}

string Clause::ToLrat() const
{
  stringstream ss;
  ss << index_ << ' ' << ToDimacs() << " 0";
  return ss.str();
}

std::ostream& operator<< (std::ostream& stream, const Clause& clause){
  if(clause.size() == 0){
    stream << "{}";
  } else {
    stream << '{' << *clause.cbegin();
    for(auto it = clause.cbegin()+1; it != clause.cend(); ++it){
      stream << ' ' << *it;
    }
    stream << '}';
  }
  return stream;
} 

} // namespace
