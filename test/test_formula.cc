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

TEST_CASE("Formula::FalsifyLiteral - Positive Literal"){
  Formula formula{2, 1};
  Clause clause{1, 2};
  clause.SetIndex(1);
  formula.AddClause(clause);
  REQUIRE(formula.TruthValue(1) == kUnassigned);
  REQUIRE(formula.TruthValue(-1) == kUnassigned);
  formula.Falsify(1);
  REQUIRE(formula.TruthValue(1) == kFalse);
  REQUIRE(formula.TruthValue(-1) == kTrue);
}

TEST_CASE("Formula::FalsifyLiteral - Negative Literal"){
  Formula formula{2, 1};
  Clause clause{1, 2};
  clause.SetIndex(1);
  formula.AddClause(clause);
  REQUIRE(formula.TruthValue(1) == kUnassigned);
  REQUIRE(formula.TruthValue(-1) == kUnassigned);
  formula.Falsify(-1);
  REQUIRE(formula.TruthValue(-1) == kFalse);
  REQUIRE(formula.TruthValue(1) == kTrue);
}

TEST_CASE("Formula::SatisfyLiteral - Positive Literal"){
  Formula formula{2, 1};
  Clause clause{1, 2};
  clause.SetIndex(1);
  formula.AddClause(clause);
  REQUIRE(formula.TruthValue(1) == kUnassigned);
  REQUIRE(formula.TruthValue(-1) == kUnassigned);
  formula.Satisfy(1);
  REQUIRE(formula.TruthValue(1) == kTrue);
  REQUIRE(formula.TruthValue(-1) == kFalse);
}

TEST_CASE("Formula::SatisfyLiteral - Negative Literal"){
  Formula formula{2, 1};
  Clause clause{1, 2};
  clause.SetIndex(1);
  formula.AddClause(clause);
  REQUIRE(formula.TruthValue(1) == kUnassigned);
  REQUIRE(formula.TruthValue(-1) == kUnassigned);
  formula.Satisfy(-1);
  REQUIRE(formula.TruthValue(-1) == kTrue);
  REQUIRE(formula.TruthValue(1) == kFalse);
}

TEST_CASE("Formula::UnassignLiteral - Positive Literal"){
  Formula formula{2, 1};
  Clause clause{1, 2};
  clause.SetIndex(1);
  formula.AddClause(clause);
  formula.Satisfy(1);
  REQUIRE(formula.TruthValue(1) == kTrue);
  REQUIRE(formula.TruthValue(-1) == kFalse);
  formula.Unassign(1);
  REQUIRE(formula.TruthValue(1) == kUnassigned);
  REQUIRE(formula.TruthValue(-1) == kUnassigned);
}

TEST_CASE("Formula::UnassignLiteral - Negative Literal"){
  Formula formula{2, 1};
  Clause clause{1, 2};
  clause.SetIndex(1);
  formula.AddClause(clause);
  formula.Satisfy(1);
  REQUIRE(formula.TruthValue(1) == kTrue);
  REQUIRE(formula.TruthValue(-1) == kFalse);
  formula.Unassign(-1);
  REQUIRE(formula.TruthValue(1) == kUnassigned);
  REQUIRE(formula.TruthValue(-1) == kUnassigned);
}

TEST_CASE("Formula::Propagate - Empty formula"){
  Formula formula{0,0};
  REQUIRE(formula.Propagate());
}

TEST_CASE("Formula::Propagate - Two complementary unit clauses"){
  Formula formula{2,2};
  Clause clause{1};
  clause.SetIndex(1);
  formula.AddClause(clause);
  Clause negated_clause{-1};
  negated_clause.SetIndex(2);
  formula.AddClause(negated_clause);
  REQUIRE(!formula.Propagate());
}

TEST_CASE("Formula::Propagate - Two clauses, no conflict"){
  Formula formula{2,2};
  Clause clause{1};
  clause.SetIndex(1);
  formula.AddClause(clause);
  Clause other{-1, 2};
  other.SetIndex(2);
  formula.AddClause(other);
  REQUIRE(formula.Propagate());
}

TEST_CASE("Formula::Propagate - Several clauses, no conflict"){
  Formula formula{2,4};
  Clause first{1};
  first.SetIndex(1);
  formula.AddClause(first);
  Clause second{2};
  second.SetIndex(2);
  formula.AddClause(second);
  Clause third{-1, 2};
  third.SetIndex(3);
  formula.AddClause(third);
  Clause fourth{-2, 1};
  fourth.SetIndex(4);
  formula.AddClause(fourth);
  REQUIRE(formula.Propagate());
}

TEST_CASE("Formula::Propagate - Chain leads to conflict, binary clauses"){
  const int number_of_clauses = 8;
  Formula formula{number_of_clauses, number_of_clauses};
  Clause first_unit{1};
  first_unit.SetIndex(1);
  formula.AddClause(first_unit);
  for(int i=2; i < number_of_clauses; i++){
    Clause binary{-(i-1), i};
    binary.SetIndex(i);
    formula.AddClause(binary);
  }
  Clause conflict{-(number_of_clauses-1)};
  conflict.SetIndex(number_of_clauses);
  formula.AddClause(conflict);
  REQUIRE(!formula.Propagate());
}

TEST_CASE("Formula::Propagate - Chain leads to conflict, no binary clauses"){
  const int number_of_clauses = 8;
  Formula formula{number_of_clauses, number_of_clauses};
  Clause first_unit{1};
  first_unit.SetIndex(1);
  formula.AddClause(first_unit);
  Clause second_unit{2};
  second_unit.SetIndex(2);
  formula.AddClause(second_unit);
  for(int i=3; i < number_of_clauses; i++){
    Clause ternary{-(i-1), -(i-2), i};
    ternary.SetIndex(i);
    formula.AddClause(ternary);
  }
  Clause conflict{-(number_of_clauses-1)};
  conflict.SetIndex(number_of_clauses);
  formula.AddClause(conflict);
  REQUIRE(!formula.Propagate());
}

TEST_CASE("Formula::Propagate - Example from Urquhart formula"){
  Formula formula{5, 6};
  Clause first_rup_unit{1};
  first_rup_unit.SetIndex(1);
  formula.AddClause(first_rup_unit);
  Clause second_rup_unit{-2};
  second_rup_unit.SetIndex(2);
  formula.AddClause(second_rup_unit);
  Clause third_rup_unit{-3};
  third_rup_unit.SetIndex(3);
  formula.AddClause(third_rup_unit);
  Clause first_existing{4, -1};
  first_existing.SetIndex(4);
  formula.AddClause(first_existing);
  Clause second_existing{5, -1, 2, 3};
  second_existing.SetIndex(5);
  formula.AddClause(second_existing);
  Clause third_existing{-4, -5};
  third_existing.SetIndex(6);
  formula.AddClause(third_existing);
  REQUIRE(!formula.Propagate());
}

TEST_CASE("Formula::Propagate - Example from two-pigeons-per-hole formula"){
  Formula formula{4, 6};
  Clause first_rup_unit{1};
  first_rup_unit.SetIndex(1);
  formula.AddClause(first_rup_unit);
  Clause second_rup_unit{2};
  second_rup_unit.SetIndex(2);
  formula.AddClause(second_rup_unit);
  Clause first_existing{-2, -3};
  first_existing.SetIndex(3);
  formula.AddClause(first_existing);
  Clause second_existing{4, -1, -2};
  second_existing.SetIndex(5);
  formula.AddClause(second_existing);
  Clause third_existing{3, -4};
  third_existing.SetIndex(6);
  formula.AddClause(third_existing);
  REQUIRE(!formula.Propagate());
}

