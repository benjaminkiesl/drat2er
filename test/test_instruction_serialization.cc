#include "catch.hpp"
#include "instruction_serialization.h"
#include "clause.h"
#include "rat_clause.h"

using namespace drat2er;

TEST_CASE("ToDIMACS(const Clause& clause) Empty Clause")
{
  Clause empty {};
  empty.SetIndex(1);
  REQUIRE(ToDIMACS(empty) == "0");
}

TEST_CASE("ToDIMACS(const Clause& clause) Unary Clause")
{
  Clause unit {-1};
  unit.SetIndex(1);
  REQUIRE(ToDIMACS(unit) == "-1 0");
}

TEST_CASE("ToDIMACS(const Clause& clause) Longer Clause")
{
  Clause clause {1, -2, -3, 4, 5};
  clause.SetIndex(1);
  REQUIRE(ToDIMACS(clause) == "1 -2 -3 4 5 0");
}

// TODO: Adapt the tests below to the new interface
//
//TEST_CASE("RupClause::ToDimacs Empty Clause"){
//  RupClause empty{};
//  empty.SetIndex(1);
//  REQUIRE(empty.ToDimacs() == "0");
//}
//
//TEST_CASE("RupClause::ToDimacs Unit Clause"){
//  RupClause unit{-1};
//  unit.SetIndex(1);
//  REQUIRE(unit.ToDimacs() == "-1 0");
//}
//
//TEST_CASE("RupClause::ToDimacs Longer Clause"){
//  RupClause clause{1, -2, -3, 4, 5};
//  clause.SetIndex(1);
//  REQUIRE(clause.ToDimacs() == "1 -2 -3 4 5 0");
//}
//
//TEST_CASE("RupClause::ToLrat Empty Clause Without Hints"){
//  RupClause empty{};
//  empty.SetIndex(1);
//  REQUIRE(empty.ToLrat() == "1 0 0");
//}
//
//TEST_CASE("RupClause::ToLrat Unit Clause Without Hints"){
//  RupClause unit{-1};
//  unit.SetIndex(1);
//  REQUIRE(unit.ToLrat() == "1 -1 0 0");
//}
//
//TEST_CASE("RupClause::ToLrat Longer Clause Without Hints"){
//  RupClause clause{1, -2, -3, 4, 5};
//  clause.SetIndex(1);
//  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 0");
//}
//
//TEST_CASE("RupClause::ToLrat Empty Clause With Hints"){
//  RupClause empty{};
//  empty.SetIndex(1);
//  empty.AddPositiveHint(4);
//  empty.AddPositiveHint(5);
//  REQUIRE(empty.ToLrat() == "1 0 4 5 0");
//}
//
//TEST_CASE("RupClause::ToLrat Unit Clause With Hints"){
//  RupClause unit{-1};
//  unit.SetIndex(1);
//  unit.AddPositiveHint(5);
//  REQUIRE(unit.ToLrat() == "1 -1 0 5 0");
//}
//
//TEST_CASE("RupClause::ToLrat Longer Clause With Hints"){
//  RupClause clause{1, -2, -3, 4, 5};
//  clause.SetIndex(1);
//  clause.AddPositiveHint(4);
//  clause.AddPositiveHint(5);
//  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 4 5 0");
//}
//
//TEST_CASE("RatClause::ToDimacs Empty Clause"){
//  RatClause empty{};
//  empty.SetIndex(1);
//  REQUIRE(empty.ToDimacs() == "0");
//}
//
//TEST_CASE("RatClause::ToDimacs Unary Clause"){
//  RatClause unit{-1};
//  unit.SetIndex(1);
//  REQUIRE(unit.ToDimacs() == "-1 0");
//}
//
//TEST_CASE("RatClause::ToDimacs Longer Clause"){
//  RatClause clause{1, -2, -3, 4, 5};
//  clause.SetIndex(1);
//  REQUIRE(clause.ToDimacs() == "1 -2 -3 4 5 0");
//}
//
//TEST_CASE("RatClause::ToLrat Empty Clause Without Hints"){
//  RatClause empty{};
//  empty.SetIndex(1);
//  REQUIRE(empty.ToLrat() == "1 0 0");
//}
//
//TEST_CASE("RatClause::ToLrat Unary Clause Without Hints"){
//  RatClause unit{-1};
//  unit.SetIndex(1);
//  REQUIRE(unit.ToLrat() == "1 -1 0 0");
//}
//
//TEST_CASE("RatClause::ToLrat Longer Clause Without Hints"){
//  RatClause clause{1, -2, -3, 4, 5};
//  clause.SetIndex(1);
//  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 0");
//}
//
//TEST_CASE("RatClause::ToLrat Empty Clause With Only Positive Hints"){
//  RatClause empty{};
//  empty.SetIndex(1);
//  empty.AddPositiveHint(4);
//  empty.AddPositiveHint(5);
//  REQUIRE(empty.ToLrat() == "1 0 4 5 0");
//}
//
//TEST_CASE("RatClause::ToLrat Unary Clause With Only Positive Hints"){
//  RatClause unit{-1};
//  unit.SetIndex(1);
//  unit.AddPositiveHint(5);
//  REQUIRE(unit.ToLrat() == "1 -1 0 5 0");
//}
//
//TEST_CASE("RatClause::ToLrat Longer Clause With Only Positive Hints"){
//  RatClause clause{1, -2, -3, 4, 5};
//  clause.SetIndex(1);
//  clause.AddPositiveHint(4);
//  clause.AddPositiveHint(5);
//  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 4 5 0");
//}
//
//TEST_CASE("RatClause::ToLrat Empty Clause With Only Negative Hints"){
//  RatClause empty{};
//  empty.SetIndex(1);
//  empty.AddNegativeHint(5, vector<int>{10, 11});
//  empty.AddNegativeHint(4, vector<int>{8,9});
//  REQUIRE(empty.ToLrat() == "1 0 -4 8 9 -5 10 11 0");
//}
//
//TEST_CASE("RatClause::ToLrat Longer Clause With Only Negative Hints"){
//  RatClause clause{1, -2, -3, 4, 5};
//  clause.SetIndex(1);
//  clause.AddNegativeHint(5, vector<int>{10, 11});
//  clause.AddNegativeHint(4, vector<int>{8,9});
//  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 -4 8 9 -5 10 11 0");
//}
//
//TEST_CASE("RatClause::ToLrat Empty Clause With Positive and Negative Hints"){
//  RatClause empty{};
//  empty.SetIndex(1);
//  empty.AddPositiveHint(3);
//  empty.AddPositiveHint(2);
//  empty.AddNegativeHint(4, vector<int>{8,9});
//  empty.AddNegativeHint(5, vector<int>{10, 11});
//  REQUIRE(empty.ToLrat() == "1 0 3 2 -4 8 9 -5 10 11 0");
//}
//
//TEST_CASE("RatClause::ToLrat Longer Clause With Positive and Negative Hints"){
//  RatClause clause{1, -2, -3, 4, 5};
//  clause.SetIndex(1);
//  clause.AddPositiveHint(3);
//  clause.AddPositiveHint(2);
//  clause.AddNegativeHint(4, vector<int>{8,9});
//  clause.AddNegativeHint(5, vector<int>{10, 11});
//  REQUIRE(clause.ToLrat() == "1 1 -2 -3 4 5 0 3 2 -4 8 9 -5 10 11 0");
//}
