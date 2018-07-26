#include "catch.hpp"
#include <vector>
#include "lrat_parser.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "deletion.h"

using std::vector;

using namespace drat2er;

TEST_CASE("LratParser::IsProperRatAddition - Empty Clause Without Hints"){
  REQUIRE(!LratParser::IsProperRatAddition("1 0 0"));
}

TEST_CASE("LratParser::IsProperRatAddition - Unit Clause Without Hints"){
  REQUIRE(LratParser::IsProperRatAddition("1 -1 0 0"));
}

TEST_CASE("LratParser::IsProperRatAddition - Longer Clause Without Hints"){
  REQUIRE(LratParser::IsProperRatAddition("1 1 -2 -3 4 5 0 0"));
}

TEST_CASE("LratParser::IsProperRatAddition - "
          "Empty Clause With Only Positive Hints"){
  REQUIRE(!LratParser::IsProperRatAddition("1 0 4 5 0"));
}

TEST_CASE("LratParser::IsProperRatAddition - "
          "Longer Clause With Only Negative Hints"){
  REQUIRE(LratParser::IsProperRatAddition("1 1 -2 -3 5 0 -4 8 9 -5 10 11 0"));
}

TEST_CASE("LratParser::IsProperRatAddition - "
          "Empty Clause With Positive and Negative Hints"){
  REQUIRE(!LratParser::IsProperRatAddition("1 0 3 2 -4 8 9 -5 10 11 0"));
}

TEST_CASE("LratParser::IsProperRatAddition - "
          "Longer Clause With Positive and Negative Hints"){
  REQUIRE(LratParser::IsProperRatAddition(
        "1 1 -2 -3 4 5 0 3 2 -4 8 9 -5 10 11 0"));
}

TEST_CASE("LratParser::ParseProperRat - " "Unit Clause Without Hints"){
  auto rat = LratParser::ParseProperRat("1 -1 0 0");
  REQUIRE(rat.GetIndex() == 1);
  REQUIRE(rat.GetLiterals() == vector<int>{-1});
  REQUIRE(rat.GetPositiveHints().empty());
  REQUIRE(rat.GetNegativeHints().empty());
}

TEST_CASE("LratParser::ParseProperRat - Longer Clause Without Hints"){
  auto rat = LratParser::ParseProperRat("1 1 -2 -3 4 5 0 0");
  REQUIRE(rat.GetIndex() == 1);
  REQUIRE(rat.GetLiterals() == vector<int>{1, -2, -3, 4, 5});
  REQUIRE(rat.GetPositiveHints().empty());
  REQUIRE(rat.GetNegativeHints().empty());
}

TEST_CASE("LratParser::ParseProperRat - "
          "Longer Clause With Only Negative Hints"){
  auto rat = LratParser::ParseProperRat("1 1 -2 -3 4 5 0 -4 8 9 -5 10 11 0");
  REQUIRE(rat.GetIndex() == 1);
  REQUIRE(rat.GetLiterals() == vector<int>{1, -2, -3, 4, 5});
  REQUIRE(rat.GetPositiveHints().empty());
  REQUIRE(rat.GetNegativeHints().size() == 2);
  REQUIRE(rat.GetNegativeHints().at(4) == vector<int>{8, 9});
  REQUIRE(rat.GetNegativeHints().at(5) == vector<int>{10, 11});
}

TEST_CASE("LratParser::ParseProperRat - "
          "Longer Clause With Positive and Negative Hints"){
  auto rat = LratParser::ParseProperRat("1 1 -2 -3 5 0 3 2 -4 8 9 -5 10 11 0");
  REQUIRE(rat.GetIndex() == 1);
  REQUIRE(rat.GetLiterals() == vector<int>{1, -2, -3, 5});
  REQUIRE(rat.GetPositiveHints() == vector<int>{3, 2});
  REQUIRE(rat.GetNegativeHints().size() == 2);
  REQUIRE(rat.GetNegativeHints().at(4) == vector<int>{8, 9});
  REQUIRE(rat.GetNegativeHints().at(5) == vector<int>{10, 11});
}

TEST_CASE("LratParser::ParseRup - Empty Clause Without Hints"){
  auto rup = LratParser::ParseRup("1 0 0");
  REQUIRE(rup.GetIndex() == 1);
  REQUIRE(rup.GetLiterals().empty());
  REQUIRE(rup.GetPositiveHints().empty());
}

TEST_CASE("LratParser::ParseRup - Unit Clause Without Hints"){
  auto rup = LratParser::ParseRup("1 -1 0 0");
  REQUIRE(rup.GetIndex() == 1);
  REQUIRE(rup.GetLiterals() == vector<int>{-1});
  REQUIRE(rup.GetPositiveHints().empty());
}

TEST_CASE("LratParser::ParseRup - Longer Clause Without Hints"){
  auto rup = LratParser::ParseRup("1 1 -2 -3 4 5 0 0");
  REQUIRE(rup.GetIndex() == 1);
  REQUIRE(rup.GetLiterals() == vector<int>{1, -2, -3, 4, 5});
  REQUIRE(rup.GetPositiveHints().empty());
}

TEST_CASE("LratParser::ParseRup - Empty Clause With Hints"){
  auto rup = LratParser::ParseRup("1 0 4 5 0");
  REQUIRE(rup.GetIndex() == 1);
  REQUIRE(rup.GetLiterals().empty());
  REQUIRE(rup.GetPositiveHints() == vector<int>{4, 5});
}

TEST_CASE("LratParser::ParseRup - Unit Clause With Hints"){
  auto rup = LratParser::ParseRup("1 -1 0 4 5 0");
  REQUIRE(rup.GetIndex() == 1);
  REQUIRE(rup.GetLiterals() == vector<int>{-1});
  REQUIRE(rup.GetPositiveHints() == vector<int>{4, 5});
}

TEST_CASE("LratParser::ParseRup - Longer Clause With Hints"){
  auto rup = LratParser::ParseRup("1 1 -2 -3 4 5 0 4 5 0");
  REQUIRE(rup.GetIndex() == 1);
  REQUIRE(rup.GetLiterals() == vector<int>{1, -2, -3, 4, 5});
  REQUIRE(rup.GetPositiveHints() == vector<int>{4, 5});
}

TEST_CASE("LratParser::ParseDeletion - Single Clause"){
  auto deletion = LratParser::ParseDeletion("1 d 2 0");
  REQUIRE(deletion.GetIndex() == 1);
  REQUIRE(deletion.GetClauseIndices() == vector<int>{2});
}

TEST_CASE("LratParser::ParseDeletion - Multiple Clauses"){
  auto deletion = LratParser::ParseDeletion("1 d 2 3 4 5 0");
  REQUIRE(deletion.GetIndex() == 1);
  REQUIRE(deletion.GetClauseIndices() == vector<int>{2, 3, 4, 5});
}
