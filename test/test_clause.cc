#include "catch.hpp"
#include <vector>
#include "clause.h"

using namespace drat2er;
using std::vector;

TEST_CASE("Clause::Clause()"){
  Clause clause{};
  REQUIRE(clause.GetIndex() == -1);
  REQUIRE(clause.GetMaxVariable() == 0);
}

TEST_CASE("Clause::operator=(Clause other)"){
  Clause original{1, 2, 3, 4};
  original.SetIndex(5);
  Clause other;
  other = original;
  REQUIRE(original.GetIndex() == 5);
  REQUIRE(other.GetIndex() == 5);
  REQUIRE(original.GetLiterals() == vector<int>{1, 2, 3, 4});
  REQUIRE(other.GetLiterals() == vector<int>{1, 2, 3, 4});
}

TEST_CASE("Clause::ContainsLiteral"){
  Clause original{1, 2, 3, 4};
  REQUIRE(original.ContainsLiteral(1));
  REQUIRE(original.ContainsLiteral(2));
  REQUIRE(original.ContainsLiteral(3));
  REQUIRE(original.ContainsLiteral(4));
  REQUIRE(!original.ContainsLiteral(0));
  REQUIRE(!original.ContainsLiteral(5));
}

TEST_CASE("Clause::ContainsLiteral Empty Clause"){
  Clause original{};
  REQUIRE(!original.ContainsLiteral(1));
}

TEST_CASE("Clause::GetMaxVariable Default Constructor + AddLiteral"){
  Clause clause;
  clause.AddLiteral(4);
  clause.AddLiteral(-6);
  clause.AddLiteral(5);
  REQUIRE(clause.GetMaxVariable() == 6);
}

TEST_CASE("Clause::GetMaxVariable Initializer List Constructor"){
  Clause clause{4, -6, 5};
  REQUIRE(clause.GetMaxVariable() == 6);
}

TEST_CASE("Clause::GetMaxVariable Empty Clause"){
  Clause clause;
  REQUIRE(clause.GetMaxVariable() == 0);
}

TEST_CASE("SwapLiteralToSecondPosition - Already there"){
  Clause clause{1, 2, 3};
  SwapLiteralToSecondPosition(2, clause);
  REQUIRE(clause.GetLiterals() == vector<int>{1, 2, 3});
}

TEST_CASE("SwapLiteralToSecondPosition - Not there"){
  Clause clause{1, 2, 3};
  SwapLiteralToSecondPosition(1, clause);
  REQUIRE(clause.GetLiterals() == vector<int>{2, 1, 3});
}
