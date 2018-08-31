#include "catch.hpp"
#include <vector>
#include "instruction_serialization.h"
#include "clause.h"
#include "rup_clause.h"
#include "rat_clause.h"

using namespace drat2er;

using std::vector;

TEST_CASE("ToDIMACS - Empty Clause")
{
  Clause empty {};
  empty.SetIndex(1);
  REQUIRE(ToDIMACS(empty) == "0");
}

TEST_CASE("ToDIMACS - Unit Clause")
{
  Clause unit {-1};
  unit.SetIndex(1);
  REQUIRE(ToDIMACS(unit) == "-1 0");
}

TEST_CASE("ToDIMACS - Longer Clause")
{
  Clause clause {1, -2, -3, 4, 5};
  clause.SetIndex(1);
  REQUIRE(ToDIMACS(clause) == "1 -2 -3 4 5 0");
}

TEST_CASE("ToDIMACS - Empty RupClause"){
  RupClause empty{};
  empty.SetIndex(1);
  REQUIRE(ToDIMACS(empty) == "0");
}

TEST_CASE("ToDIMACS - Unit RupClause"){
  RupClause unit{-1};
  unit.SetIndex(1);
  REQUIRE(ToDIMACS(unit) == "-1 0");
}

TEST_CASE("ToDIMACS - Longer RupClause"){
  RupClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  REQUIRE(ToDIMACS(clause) == "1 -2 -3 4 5 0");
}

TEST_CASE("ToLRAT - Empty RupClause With Hints"){
  RupClause empty{};
  empty.SetIndex(1);
  empty.AddPositiveHint(4);
  empty.AddPositiveHint(5);
  REQUIRE(ToLRAT(empty) == "1 0 4 5 0");
}

TEST_CASE("ToLRAT - Unit RupClause With Hints"){
  RupClause unit{-1};
  unit.SetIndex(1);
  unit.AddPositiveHint(5);
  REQUIRE(ToLRAT(unit) == "1 -1 0 5 0");
}

TEST_CASE("ToLRAT - Longer RupClause With Hints"){
  RupClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  clause.AddPositiveHint(4);
  clause.AddPositiveHint(5);
  REQUIRE(ToLRAT(clause) == "1 1 -2 -3 4 5 0 4 5 0");
}

TEST_CASE("ToDIMACS - Empty RatClause"){
  RatClause empty{};
  empty.SetIndex(1);
  REQUIRE(ToDIMACS(empty) == "0");
}

TEST_CASE("ToDIMACS - Unary RatClause"){
  RatClause unit{-1};
  unit.SetIndex(1);
  REQUIRE(ToDIMACS(unit) == "-1 0");
}

TEST_CASE("ToDIMACS - Longer RatClause"){
  RatClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  REQUIRE(ToDIMACS(clause) == "1 -2 -3 4 5 0");
}

TEST_CASE("ToLRAT - Empty RatClause Without Hints"){
  RatClause empty{};
  empty.SetIndex(1);
  REQUIRE(ToLRAT(empty) == "1 0 0");
}

TEST_CASE("ToLRAT - Unary RatClause Without Hints"){
  RatClause unit{-1};
  unit.SetIndex(1);
  REQUIRE(ToLRAT(unit) == "1 -1 0 0");
}

TEST_CASE("ToLRAT - Longer RatClause Without Hints"){
  RatClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  REQUIRE(ToLRAT(clause) == "1 1 -2 -3 4 5 0 0");
}

TEST_CASE("ToLRAT - Empty RatClause With Only Positive Hints"){
  RatClause empty{};
  empty.SetIndex(1);
  empty.AddPositiveHint(4);
  empty.AddPositiveHint(5);
  REQUIRE(ToLRAT(empty) == "1 0 4 5 0");
}

TEST_CASE("ToLRAT - Unary RatClause With Only Positive Hints"){
  RatClause unit{-1};
  unit.SetIndex(1);
  unit.AddPositiveHint(5);
  REQUIRE(ToLRAT(unit) == "1 -1 0 5 0");
}

TEST_CASE("ToLRAT - Longer RatClause With Only Positive Hints"){
  RatClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  clause.AddPositiveHint(4);
  clause.AddPositiveHint(5);
  REQUIRE(ToLRAT(clause) == "1 1 -2 -3 4 5 0 4 5 0");
}

TEST_CASE("ToLRAT - Empty RatClause With Only Negative Hints"){
  RatClause empty{};
  empty.SetIndex(1);
  empty.AddNegativeHint(5, vector<int>{10, 11});
  empty.AddNegativeHint(4, vector<int>{8,9});
  REQUIRE(ToLRAT(empty) == "1 0 -4 8 9 -5 10 11 0");
}

TEST_CASE("ToLRAT - Longer RatClause With Only Negative Hints"){
  RatClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  clause.AddNegativeHint(5, vector<int>{10, 11});
  clause.AddNegativeHint(4, vector<int>{8,9});
  REQUIRE(ToLRAT(clause) == "1 1 -2 -3 4 5 0 -4 8 9 -5 10 11 0");
}

TEST_CASE("ToLRAT - Empty RatClause With Positive and Negative Hints"){
  RatClause empty{};
  empty.SetIndex(1);
  empty.AddPositiveHint(3);
  empty.AddPositiveHint(2);
  empty.AddNegativeHint(4, vector<int>{8,9});
  empty.AddNegativeHint(5, vector<int>{10, 11});
  REQUIRE(ToLRAT(empty) == "1 0 3 2 -4 8 9 -5 10 11 0");
}

TEST_CASE("ToLRAT - Longer RatClause With Positive and Negative Hints"){
  RatClause clause{1, -2, -3, 4, 5};
  clause.SetIndex(1);
  clause.AddPositiveHint(3);
  clause.AddPositiveHint(2);
  clause.AddNegativeHint(4, vector<int>{8,9});
  clause.AddNegativeHint(5, vector<int>{10, 11});
  REQUIRE(ToLRAT(clause) == "1 1 -2 -3 4 5 0 3 2 -4 8 9 -5 10 11 0");
}
