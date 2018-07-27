#include "catch.hpp"
#include <vector>
#include <algorithm>
#include "formula.h"

using namespace drat2er;
using std::vector;
using std::find;

TEST_CASE("Formula::AddClause - Add and Obtain"){
  Formula formula{100, 100};
  const int clause_index = 5;
  Clause clause{1, 2};
  clause.SetIndex(clause_index);
  REQUIRE(formula.GetClause(clause_index) == nullptr);
  formula.AddClause(clause);
  REQUIRE(formula.GetClause(clause_index) != nullptr);
  REQUIRE(formula.GetClause(clause_index)->GetLiterals() == vector<int>{1, 2});
  REQUIRE(formula.GetClause(clause_index)->GetIndex() == clause_index);
}

TEST_CASE("Formula::AddClause - Check that occurrences are updated"){
  Formula formula{100, 100};
  REQUIRE(formula.Occurrences(1).empty());
  REQUIRE(formula.Occurrences(2).empty());
  REQUIRE(formula.Occurrences(-1).empty());
  REQUIRE(formula.Occurrences(3).empty());
  Clause first_clause{1, 2};
  first_clause.SetIndex(1);
  Clause second_clause{-1, 3};
  second_clause.SetIndex(2);

  formula.AddClause(first_clause);
  formula.AddClause(second_clause);

  auto first_clause_ptr = formula.GetClause(first_clause.GetIndex());
  REQUIRE(find(formula.Occurrences(1).begin(), formula.Occurrences(1).end(), 
               first_clause_ptr) != formula.Occurrences(1).end());
  REQUIRE(find(formula.Occurrences(2).begin(), formula.Occurrences(2).end(), 
               first_clause_ptr) != formula.Occurrences(2).end());
  REQUIRE(find(formula.Occurrences(-1).begin(), formula.Occurrences(-1).end(), 
               first_clause_ptr) == formula.Occurrences(-1).end());
  REQUIRE(find(formula.Occurrences(3).begin(), formula.Occurrences(3).end(), 
               first_clause_ptr) == formula.Occurrences(3).end());
 
  auto second_clause_ptr = formula.GetClause(second_clause.GetIndex());
  REQUIRE(find(formula.Occurrences(-1).begin(), formula.Occurrences(-1).end(), 
               second_clause_ptr) != formula.Occurrences(-1).end());
  REQUIRE(find(formula.Occurrences(3).begin(), formula.Occurrences(3).end(), 
               second_clause_ptr) != formula.Occurrences(3).end());
  REQUIRE(find(formula.Occurrences(1).begin(), formula.Occurrences(1).end(), 
               second_clause_ptr) == formula.Occurrences(1).end());
  REQUIRE(find(formula.Occurrences(2).begin(), formula.Occurrences(2).end(), 
               second_clause_ptr) == formula.Occurrences(2).end());
}

TEST_CASE("Formula::DeleteClause - Check that clause itself is removed"){
  Formula formula{100, 100};
  Clause clause{1, 2};
  clause.SetIndex(1);
  REQUIRE(formula.GetClause(clause.GetIndex()) == nullptr);
  formula.AddClause(clause);
  REQUIRE(formula.GetClause(clause.GetIndex()) != nullptr);
  formula.DeleteClause(clause.GetIndex());
  REQUIRE(formula.GetClause(clause.GetIndex()) == nullptr);
}

TEST_CASE("Formula::DeleteClause - Check that occurrences are updated"){
  Formula formula{100, 100};
  REQUIRE(formula.Occurrences(1).empty());
  REQUIRE(formula.Occurrences(2).empty());
  Clause clause{1, 2};
  clause.SetIndex(1);
  formula.AddClause(clause);
  REQUIRE(formula.Occurrences(1).size() == 1);
  REQUIRE(formula.Occurrences(1).front()->GetIndex() == clause.GetIndex());
  REQUIRE(formula.Occurrences(2).size() == 1);
  REQUIRE(formula.Occurrences(2).front()->GetIndex() == clause.GetIndex());
  formula.DeleteClause(clause.GetIndex());
  REQUIRE(formula.Occurrences(1).empty());
  REQUIRE(formula.Occurrences(2).empty());
}

