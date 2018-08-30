#include "catch.hpp"
#include <vector>
#include <map>
#include "rat_clause.h"

using namespace drat2er;
using std::vector;
using std::map;

TEST_CASE("RatClause::RatClause(RatClause&& other)"){
  RatClause original{1, 2, 3, 4};
  original.SetIndex(5);
  original.AddPositiveHint(8);
  original.AddPositiveHint(9);
  original.AddNegativeHint(6, vector<int>{2,3,4});
  original.AddNegativeHint(7, vector<int>{8,9});
  RatClause other(original);
  REQUIRE(original.GetIndex() == 5);
  REQUIRE(other.GetIndex() == original.GetIndex());
  REQUIRE(original.GetLiterals() == vector<int>{1, 2, 3, 4});
  REQUIRE(other.GetLiterals() == original.GetLiterals());
  REQUIRE(original.GetPositiveHints() == vector<int>{8, 9});
  REQUIRE(other.GetPositiveHints() == original.GetPositiveHints());
  REQUIRE(other.GetNegativeHints().at(6) == vector<int>{2,3,4});
  REQUIRE(original.GetNegativeHints().at(6) == other.GetNegativeHints().at(6));
  REQUIRE(other.GetNegativeHints().at(7) == vector<int>{8,9});
  REQUIRE(original.GetNegativeHints().at(7) == other.GetNegativeHints().at(7));
}

TEST_CASE("RatClause::operator=(RatClause other)"){
  RatClause original{1, 2, 3, 4};
  original.SetIndex(5);
  original.AddPositiveHint(8);
  original.AddPositiveHint(9);
  original.AddNegativeHint(6, vector<int>{2,3,4});
  original.AddNegativeHint(7, vector<int>{8,9});
  RatClause other;
  other = original;
  REQUIRE(original.GetIndex() == 5);
  REQUIRE(other.GetIndex() == original.GetIndex());
  REQUIRE(original.GetLiterals() == vector<int>{1, 2, 3, 4});
  REQUIRE(other.GetLiterals() == original.GetLiterals());
  REQUIRE(original.GetPositiveHints() == vector<int>{8, 9});
  REQUIRE(other.GetPositiveHints() == original.GetPositiveHints());
  REQUIRE(other.GetNegativeHints().at(6) == vector<int>{2,3,4});
  REQUIRE(original.GetNegativeHints().at(6) == other.GetNegativeHints().at(6));
  REQUIRE(other.GetNegativeHints().at(7) == vector<int>{8,9});
  REQUIRE(original.GetNegativeHints().at(7) == other.GetNegativeHints().at(7));
}

