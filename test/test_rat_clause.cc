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


TEST_CASE("RatClause::ToDimacs Empty Clause"){
  RatClause empty{};
  empty.SetIndex(1);
  REQUIRE(empty.ToDimacs() == "0");
}

TEST_CASE("RatClause::ToDimacs Unary Clause"){
  RatClause unit{-1};
  unit.SetIndex(1);
  REQUIRE(unit.ToDimacs() == "-1 0");
}

TEST_CASE("RatClause::ToDimacs Longer Clause"){
  RatClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  REQUIRE(clause.ToDimacs() == "1 -2 -3 4 5 0");
}

TEST_CASE("RatClause::ToLrat Empty Clause Without Hints"){
  RatClause empty{};
  empty.SetIndex(1);
  REQUIRE(empty.ToLrat() == "1 0 0");
}

TEST_CASE("RatClause::ToLrat Unary Clause Without Hints"){
  RatClause unit{-1};
  unit.SetIndex(1);
  REQUIRE(unit.ToLrat() == "1 -1 0 0");
}

TEST_CASE("RatClause::ToLrat Longer Clause Without Hints"){
  RatClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 0");
}

TEST_CASE("RatClause::ToLrat Empty Clause With Only Positive Hints"){
  RatClause empty{};
  empty.SetIndex(1);
  empty.AddPositiveHint(4);
  empty.AddPositiveHint(5);
  REQUIRE(empty.ToLrat() == "1 0 4 5 0");
}

TEST_CASE("RatClause::ToLrat Unary Clause With Only Positive Hints"){
  RatClause unit{-1};
  unit.SetIndex(1);
  unit.AddPositiveHint(5);
  REQUIRE(unit.ToLrat() == "1 -1 0 5 0");
}

TEST_CASE("RatClause::ToLrat Longer Clause With Only Positive Hints"){
  RatClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  clause.AddPositiveHint(4);
  clause.AddPositiveHint(5);
  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 4 5 0");
}

TEST_CASE("RatClause::ToLrat Empty Clause With Only Negative Hints"){
  RatClause empty{};
  empty.SetIndex(1);
  empty.AddNegativeHint(5, vector<int>{10, 11});
  empty.AddNegativeHint(4, vector<int>{8,9});
  REQUIRE(empty.ToLrat() == "1 0 -4 8 9 -5 10 11 0");
}

TEST_CASE("RatClause::ToLrat Longer Clause With Only Negative Hints"){
  RatClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  clause.AddNegativeHint(5, vector<int>{10, 11});
  clause.AddNegativeHint(4, vector<int>{8,9});
  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 -4 8 9 -5 10 11 0");
}

TEST_CASE("RatClause::ToLrat Empty Clause With Positive and Negative Hints"){
  RatClause empty{};
  empty.SetIndex(1);
  empty.AddPositiveHint(3);
  empty.AddPositiveHint(2);
  empty.AddNegativeHint(4, vector<int>{8,9});
  empty.AddNegativeHint(5, vector<int>{10, 11});
  REQUIRE(empty.ToLrat() == "1 0 3 2 -4 8 9 -5 10 11 0");
}

TEST_CASE("RatClause::ToLrat Longer Clause With Positive and Negative Hints"){
  RatClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  clause.AddPositiveHint(3);
  clause.AddPositiveHint(2);
  clause.AddNegativeHint(4, vector<int>{8,9});
  clause.AddNegativeHint(5, vector<int>{10, 11});
  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 3 2 -4 8 9 -5 10 11 0");
}
