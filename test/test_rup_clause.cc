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


TEST_CASE("RupClause::ToDimacs Empty Clause"){
  RupClause empty{};
  empty.SetIndex(1);
  REQUIRE(empty.ToDimacs() == "0");
}

TEST_CASE("RupClause::ToDimacs Unit Clause"){
  RupClause unit{-1};
  unit.SetIndex(1);
  REQUIRE(unit.ToDimacs() == "-1 0");
}

TEST_CASE("RupClause::ToDimacs Longer Clause"){
  RupClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  REQUIRE(clause.ToDimacs() == "1 -2 -3 4 5 0");
}

TEST_CASE("RupClause::ToLrat Empty Clause Without Hints"){
  RupClause empty{};
  empty.SetIndex(1);
  REQUIRE(empty.ToLrat() == "1 0 0");
}

TEST_CASE("RupClause::ToLrat Unit Clause Without Hints"){
  RupClause unit{-1};
  unit.SetIndex(1);
  REQUIRE(unit.ToLrat() == "1 -1 0 0");
}

TEST_CASE("RupClause::ToLrat Longer Clause Without Hints"){
  RupClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 0");
}

TEST_CASE("RupClause::ToLrat Empty Clause With Hints"){
  RupClause empty{};
  empty.SetIndex(1);
  empty.AddPositiveHint(4);
  empty.AddPositiveHint(5);
  REQUIRE(empty.ToLrat() == "1 0 4 5 0");
}

TEST_CASE("RupClause::ToLrat Unit Clause With Hints"){
  RupClause unit{-1};
  unit.SetIndex(1);
  unit.AddPositiveHint(5);
  REQUIRE(unit.ToLrat() == "1 -1 0 5 0");
}

TEST_CASE("RupClause::ToLrat Longer Clause With Hints"){
  RupClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  clause.AddPositiveHint(4);
  clause.AddPositiveHint(5);
  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 4 5 0");
}
