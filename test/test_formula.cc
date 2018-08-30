#include "catch.hpp"
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "formula.h"
#include "clause.h"
#include "rup_clause.h"
#include "instruction_serialization.h"

using namespace drat2er;
using std::vector;
using std::unordered_set;
using std::find;

TEST_CASE("Formula::AddClause - Add and Obtain"){
  Formula formula{};
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
  Formula formula{};
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
  Formula formula{};
  Clause clause{1, 2};
  clause.SetIndex(1);
  REQUIRE(formula.GetClause(clause.GetIndex()) == nullptr);
  formula.AddClause(clause);
  REQUIRE(formula.GetClause(clause.GetIndex()) != nullptr);
  formula.DeleteClause(clause.GetIndex());
  REQUIRE(formula.GetClause(clause.GetIndex()) == nullptr);
}

TEST_CASE("Formula::DeleteClause - Check that occurrences are updated"){
  Formula formula{};
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
  Formula formula{};
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
  Formula formula{};
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
  Formula formula{};
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
  Formula formula{};
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
  Formula formula{};
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
  Formula formula{};
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
  Formula formula{};
  REQUIRE(formula.Propagate());
}

TEST_CASE("Formula::Propagate - Two complementary unit clauses"){
  Formula formula{};
  Clause clause{1};
  clause.SetIndex(1);
  formula.AddClause(clause);
  Clause negated_clause{-1};
  negated_clause.SetIndex(2);
  formula.AddClause(negated_clause);
  REQUIRE(!formula.Propagate());
}

TEST_CASE("Formula::Propagate - Two clauses, no conflict"){
  Formula formula{};
  Clause clause{1};
  clause.SetIndex(1);
  formula.AddClause(clause);
  Clause other{-1, 2};
  other.SetIndex(2);
  formula.AddClause(other);
  REQUIRE(formula.Propagate());
}

TEST_CASE("Formula::Propagate - Several clauses, no conflict"){
  Formula formula{};
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
  Formula formula{};
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
  Formula formula{};
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

TEST_CASE("Formula::Propagate - Example from two-pigeons-per-hole formula"){
  Formula formula{};
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

TEST_CASE("Formula::Propagate - Example from Urquhart formula"){
  Formula formula{};
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

TEST_CASE("Formula::DeriveSubsumingClause - Example from Urquhart formula"){
  Formula formula{};
  Clause first_existing{4, -1};
  first_existing.SetIndex(4);
  formula.AddClause(first_existing);
  Clause second_existing{5, -1, 2, 3};
  second_existing.SetIndex(5);
  formula.AddClause(second_existing);
  Clause third_existing{-4, -5};
  third_existing.SetIndex(6);
  formula.AddClause(third_existing);
  Clause rup{-1, 2, 3};
  rup.SetIndex(7);
  auto resulting_clause = formula.DeriveSubsumingClause(rup);
  unordered_set<int> resulting_literals(resulting_clause->cbegin(), 
                                        resulting_clause->cend());
  unordered_set<int> desired_literals{-1, 2, 3};
  REQUIRE(resulting_literals == desired_literals);
  REQUIRE(resulting_clause->GetIndex() == rup.GetIndex());
}

TEST_CASE("Formula::DeriveSubsumingClause - Example from Urquhart formula"
          " with subsumption"){
  Formula formula{};
  Clause first_existing{4, -1};
  first_existing.SetIndex(4);
  formula.AddClause(first_existing);
  Clause second_existing{5, -1, 2, 3};
  second_existing.SetIndex(5);
  formula.AddClause(second_existing);
  Clause third_existing{-4, -5};
  third_existing.SetIndex(6);
  formula.AddClause(third_existing);
  Clause rup{-1, 2, 3, 6};
  rup.SetIndex(7);
  auto resulting_clause = formula.DeriveSubsumingClause(rup);
  unordered_set<int> resulting_literals(resulting_clause->cbegin(), 
                                        resulting_clause->cend());
  unordered_set<int> desired_literals{-1, 2, 3};
  REQUIRE(resulting_literals == desired_literals);
  REQUIRE(resulting_clause->GetIndex() == rup.GetIndex());
}

TEST_CASE("Formula::DeriveSubsumingClause"
          " - Example from a proof of the hole20 formula."){
  Clause a{548, -547};
  a.SetIndex(4402);
  Clause b{547, -545};
  b.SetIndex(4400);
  Clause c{546, 547};
  c.SetIndex(4401);
  Clause d{547, 539};
  d.SetIndex(32827);
  Clause e{547, 532};
  e.SetIndex(32826);
  Clause f{547, 525};
  f.SetIndex(32825);
  Clause g{547, 518};
  g.SetIndex(32824);
  Clause h{547, 511};
  h.SetIndex(32823);
  Clause i{547, 504};
  i.SetIndex(32822);
  Clause j{547, 497};
  j.SetIndex(32821);
  Clause k{547, 490};
  k.SetIndex(32820);
  Clause l{547, 483};
  l.SetIndex(32819);
  Clause m{547, 476};
  m.SetIndex(32818);
  Clause n{547, 469};
  n.SetIndex(32817);
  Clause o{547, 462};
  o.SetIndex(32816);
  Clause p{547, 455};
  p.SetIndex(32815);
  Clause q{547, 448};
  q.SetIndex(32814);
  Clause r{547, 441};
  r.SetIndex(32813);
  Clause s{547, 434};
  s.SetIndex(32812);
  Clause t{547, 427};
  t.SetIndex(32811);
  Clause u{-476, -525, -532, -455, -469, -546, 545, -427, -441, -462, 
           -511, -434, -518, -490, -448, -497, 419, -539, -504, -483};
  u.SetIndex(32789);
  Formula formula{};
  formula.AddClause(a);
  formula.AddClause(b);
  formula.AddClause(c);
  formula.AddClause(d);
  formula.AddClause(e);
  formula.AddClause(f);
  formula.AddClause(g);
  formula.AddClause(h);
  formula.AddClause(i);
  formula.AddClause(j);
  formula.AddClause(k);
  formula.AddClause(l);
  formula.AddClause(m);
  formula.AddClause(n);
  formula.AddClause(o);
  formula.AddClause(p);
  formula.AddClause(q);
  formula.AddClause(r);
  formula.AddClause(s);
  formula.AddClause(t);
  formula.AddClause(u);
  Clause rup{548, 419};
  rup.SetIndex(4403);
  auto subsuming = formula.DeriveSubsumingClause(rup);
  REQUIRE(subsuming != nullptr);
  INFO("RUP is '" + ToDIMACS(rup) + "', " 
       "subsuming is '" + ToDIMACS(*subsuming) + "'");
  REQUIRE(subsuming->IsSubclauseOf(rup));
}
