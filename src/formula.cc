#include "formula.h"
#include <memory>
#include <iostream>

using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::move;
using std::cerr;
using std::endl;

namespace drat2er
{

Formula::Formula(int number_of_variables, int number_of_clauses) :
clauses_(number_of_variables),
occurrences_(2*number_of_variables,std::vector<shared_ptr<Clause>> {})
{
}

void Formula::AddClause(unique_ptr<Clause> clause)
{
  shared_ptr<Clause> new_clause {move(clause)};
  clauses_.emplace_back(new_clause);
  for(auto literal : *new_clause) {
    Occurrences(literal).emplace_back(new_clause);
  }
}

std::shared_ptr<Clause> Formula::GetClause(const int clause_index) const
{
  return clauses_[clause_index];
}

void Formula::DeleteClause(const int clause_index)
{
  clauses_.erase(clauses_.begin()+clause_index);
}

vector<shared_ptr<Clause>>& Formula::Occurrences(int literal)
{
  int index = literal > 0 ? 2*literal - 2 : (-2)*literal - 1;
  if(index >= occurrences_.size()) {
    cerr << "Index is out of bounds [0," << occurrences_.size() << "): "
    << index << endl;
  }
  return occurrences_[index];
}

} // namespace
