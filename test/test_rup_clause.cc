#include "catch.hpp"
#include <vector>
#include "rup_clause.h"

using namespace drat2er;
using std::vector;

TEST_CASE("RupClause::RupClause(RupClause&& other)"){
  RupClause original{1, 2, 3, 4};
  original.SetIndex(5);
  original.AddPositiveHint(8);
  original.AddPositiveHint(9);
  RupClause other(original);
  REQUIRE(original.GetIndex() == 5);
  REQUIRE(other.GetIndex() == 5);
  REQUIRE(original.GetLiterals() == vector<int>{1, 2, 3, 4});
  REQUIRE(other.GetLiterals() == vector<int>{1, 2, 3, 4});
  REQUIRE(original.GetPositiveHints() == vector<int>{8, 9});
  REQUIRE(other.GetPositiveHints() == vector<int>{8, 9});
}

TEST_CASE("RupClause::operator=(RupClause other)"){
  RupClause original{1, 2, 3, 4};
  original.SetIndex(5);
  original.AddPositiveHint(8);
  original.AddPositiveHint(9);
  RupClause other;
  other = original;
  REQUIRE(original.GetIndex() == 5);
  REQUIRE(other.GetIndex() == 5);
  REQUIRE(original.GetLiterals() == vector<int>{1, 2, 3, 4});
  REQUIRE(other.GetLiterals() == vector<int>{1, 2, 3, 4});
  REQUIRE(original.GetPositiveHints() == vector<int>{8, 9});
  REQUIRE(other.GetPositiveHints() == vector<int>{8, 9});
}

