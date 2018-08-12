#include "catch.hpp"
#include <vector>
#include "rat_eliminator.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "deletion.h"

using namespace drat2er;
using std::vector;

TEST_CASE("RatEliminator::FirstDefinitionClause Ternary RAT"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = 1;
  const int new_variable = 4;
  RatClause rat{pivot, 2, 3};
  Clause expected{new_variable, 2, 3};
  
  auto definition = eliminator.FirstDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::FirstDefinitionClause Ternary RAT, Negated Pivot"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = -1;
  const int new_variable = 4;
  RatClause rat{pivot, 2, 3};
  Clause expected{new_variable, 2, 3};
  
  auto definition = eliminator.FirstDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::FirstDefinitionClause Unit RAT"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = 1;
  const int new_variable = 2;
  RatClause rat{pivot};
  Clause expected{2};
  
  auto definition = eliminator.FirstDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::FirstDefinitionClause Unit RAT, Negated Pivot"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = -1;
  const int new_variable = 2;
  RatClause rat{pivot};
  Clause expected{2};
  
  auto definition = eliminator.FirstDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::SecondDefinitionClause Ternary RAT"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = 1;
  const int new_variable = 4;
  RatClause rat{pivot, 2, 3};
  Clause expected{new_variable, -pivot};
  
  auto definition = eliminator.SecondDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::SecondDefinitionClause Ternary RAT, Negated Pivot"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = -1;
  const int new_variable = 4;
  RatClause rat{pivot, 2, 3};
  Clause expected{new_variable, -pivot};
  
  auto definition = eliminator.SecondDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::SecondDefinitionClause Unit RAT"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = 1;
  const int new_variable = 2;
  RatClause rat{pivot};
  Clause expected{new_variable, -pivot};
  
  auto definition = eliminator.SecondDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::SecondDefinitionClause Unit RAT, Negated Pivot"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = -1;
  const int new_variable = 2;
  RatClause rat{pivot};
  Clause expected{new_variable, -pivot};
  
  auto definition = eliminator.SecondDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::ThirdBlockOfDefinitionClauses Ternary RAT"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = 1;
  const int new_variable = 4;
  RatClause rat{pivot, 2, 3};
  
  auto definition_clauses = 
    eliminator.ThirdBlockOfDefinitionClauses(rat, new_variable);

  REQUIRE(definition_clauses.size() == 2);
  REQUIRE(definition_clauses[0].GetLiterals() == 
      vector<int>{-new_variable, pivot, -2});
  REQUIRE(definition_clauses[1].GetLiterals() == 
      vector<int>{-new_variable, pivot, -3});
}

TEST_CASE("RatEliminator::ThirdBlockOfDefinitionClauses Ternary RAT"
          ", Negated Pivot"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = -1;
  const int new_variable = 4;
  RatClause rat{pivot, 2, 3};
  
  auto definition_clauses = 
    eliminator.ThirdBlockOfDefinitionClauses(rat, new_variable);

  REQUIRE(definition_clauses.size() == 2);
  REQUIRE(definition_clauses[0].GetLiterals() == 
      vector<int>{-new_variable, pivot, -2});
  REQUIRE(definition_clauses[1].GetLiterals() == 
      vector<int>{-new_variable, pivot, -3});
}

TEST_CASE("RatEliminator::ThirdBlockOfDefinitionClauses Unit RAT"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = 1;
  const int new_variable = 2;
  RatClause rat{pivot};
  
  REQUIRE(eliminator.ThirdBlockOfDefinitionClauses(rat, new_variable).empty());
}

TEST_CASE("RatEliminator::ThirdBlockOfDefinitionClauses Unit RAT"
          ", Negated Pivot"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int pivot = -1;
  const int new_variable = 2;
  RatClause rat{pivot};
  
  REQUIRE(eliminator.ThirdBlockOfDefinitionClauses(rat, new_variable).empty());
}

TEST_CASE("RatEliminator::CorrespondingDefinition Ternary RAT"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int new_variable = 4;
  RatClause rat{1, 2, 3};
  
  auto definition = eliminator.CorrespondingDefinition(rat, new_variable);
  REQUIRE(definition.size() == 4);
  REQUIRE(definition[0].GetLiterals() == 
    eliminator.FirstDefinitionClause(rat, new_variable).GetLiterals());
  REQUIRE(definition[1].GetLiterals() == 
    eliminator.SecondDefinitionClause(rat, new_variable).GetLiterals());
  auto third_block = 
    eliminator.ThirdBlockOfDefinitionClauses(rat, new_variable);
  REQUIRE(definition[2].GetLiterals() == third_block[0].GetLiterals());
  REQUIRE(definition[3].GetLiterals() == third_block[1].GetLiterals());
}

TEST_CASE("RatEliminator::CorrespondingDefinition Unary RAT"){
  RatEliminator eliminator(nullptr, 0, 0);
  const int new_variable = 2;
  RatClause rat{1};
  
  auto definition = eliminator.CorrespondingDefinition(rat, new_variable);
  REQUIRE(definition.size() == 2);
  REQUIRE(definition[0].GetLiterals() == 
    eliminator.FirstDefinitionClause(rat, new_variable).GetLiterals());
  REQUIRE(definition[1].GetLiterals() == 
    eliminator.SecondDefinitionClause(rat, new_variable).GetLiterals());
}

TEST_CASE("RatEliminator::RenameLiteral - Single renaming"){
  RatEliminator eliminator(nullptr, 0, 0);
  REQUIRE(1 == eliminator.RenameLiteral(1));
  eliminator.UpdateLiteralRenaming(1, 2);
  REQUIRE(2 == eliminator.RenameLiteral(1));
}

TEST_CASE("RatEliminator::RenameLiteral - Double renaming"){
  RatEliminator eliminator(nullptr, 0, 0);
  REQUIRE(1 == eliminator.RenameLiteral(1));
  eliminator.UpdateLiteralRenaming(1, 2);
  eliminator.UpdateLiteralRenaming(2, 3);
  REQUIRE(3 == eliminator.RenameLiteral(1));
}

TEST_CASE("RatEliminator::RenameLiteral - Quadruple renaming"){
  RatEliminator eliminator(nullptr, 0, 0);
  REQUIRE(1 == eliminator.RenameLiteral(1));
  eliminator.UpdateLiteralRenaming(1, 2);
  eliminator.UpdateLiteralRenaming(2, 3);
  eliminator.UpdateLiteralRenaming(3, 4);
  eliminator.UpdateLiteralRenaming(4, 5);
  REQUIRE(5 == eliminator.RenameLiteral(1));
}

TEST_CASE("RatEliminator::RenameRup - Literal Renaming"){
  RatEliminator eliminator(nullptr, 0, 0);
  RupClause original{1, 2, 3};
  REQUIRE(eliminator.RenameRup(original).GetLiterals() == 
      original.GetLiterals()); 
  eliminator.UpdateLiteralRenaming(1, 4);
  eliminator.UpdateLiteralRenaming(4, 5);
  eliminator.UpdateLiteralRenaming(5, 6);
  eliminator.UpdateLiteralRenaming(2, 7);
  REQUIRE(eliminator.RenameRup(original).GetLiterals() == 
      vector<int>{6, 7, 3});
}

TEST_CASE("RatEliminator::RenameRup - Positive Hints"){
  RatEliminator eliminator(nullptr, 0, 0);
  RupClause original{1, 2, 3};
  original.AddPositiveHint(4);
  original.AddPositiveHint(5);
  original.AddPositiveHint(6);
  REQUIRE(eliminator.RenameRup(original).GetPositiveHints() == 
      vector<int>{4, 5, 6}); 
  eliminator.UpdateClauseRenaming(4, 7);
  eliminator.UpdateClauseRenaming(5, 8);
  eliminator.UpdateClauseRenaming(6, 9);
  eliminator.UpdateClauseRenaming(9, 10);
  eliminator.UpdateClauseRenaming(10, 11);
  REQUIRE(eliminator.RenameRup(original).GetPositiveHints() == 
      vector<int>{7, 8, 11});
}

TEST_CASE("RatEliminator::RenameRat - Negative Hints"){
  RatEliminator eliminator(nullptr, 0, 0);
  RatClause original{1, 2, 3};
  original.AddPositiveHint(4);
  original.AddPositiveHint(5);
  original.AddPositiveHint(6);
  REQUIRE(eliminator.RenameRat(original).GetPositiveHints() == 
      vector<int>{4, 5, 6}); 
  eliminator.UpdateClauseRenaming(4, 7);
  eliminator.UpdateClauseRenaming(5, 8);
  eliminator.UpdateClauseRenaming(6, 9);
  eliminator.UpdateClauseRenaming(9, 10);
  eliminator.UpdateClauseRenaming(10, 11);
  REQUIRE(eliminator.RenameRat(original).GetPositiveHints() == 
      vector<int>{7, 8, 11});
}

TEST_CASE("RatEliminator::RenameRat - Positive Hints"){
  RatEliminator eliminator(nullptr, 0, 0);
  RatClause original{1, 2, 3};
  original.AddNegativeHint(4, vector<int>{5, 6, 7});
  original.AddNegativeHint(8, vector<int>{9, 10, 11});
  REQUIRE(eliminator.RenameRat(original).GetNegativeHints().size() == 2);
  REQUIRE(eliminator.RenameRat(original).GetNegativeHints().at(4) ==
      vector<int>{5, 6, 7});
  REQUIRE(eliminator.RenameRat(original).GetNegativeHints().at(8) ==
      vector<int>{9, 10, 11});
  eliminator.UpdateClauseRenaming(4, 12);
  eliminator.UpdateClauseRenaming(5, 13);
  eliminator.UpdateClauseRenaming(6, 14);
  eliminator.UpdateClauseRenaming(14, 15);
  eliminator.UpdateClauseRenaming(15, 16);
  eliminator.UpdateClauseRenaming(10, 17);
  REQUIRE(eliminator.RenameRat(original).GetNegativeHints().size() == 2);
  REQUIRE(eliminator.RenameRat(original).GetNegativeHints().at(12) ==
      vector<int>{13, 16, 7});
  REQUIRE(eliminator.RenameRat(original).GetNegativeHints().at(8) ==
      vector<int>{9, 17, 11});
}

TEST_CASE("RatEliminator::RenameDeletion") {
  RatEliminator eliminator(nullptr, 0, 0);
  Deletion original;
  original.SetIndex(1);
  original.AddClauseIndex(2);
  original.AddClauseIndex(3);
  original.AddClauseIndex(4);
  Deletion renamed = eliminator.RenameDeletion(original);
  REQUIRE(renamed.GetIndex() == 1);
  REQUIRE(renamed.GetClauseIndices() == vector<int>{2, 3, 4});
  eliminator.UpdateClauseRenaming(2, 5);
  eliminator.UpdateClauseRenaming(3, 6);
  eliminator.UpdateClauseRenaming(6, 7);
  eliminator.UpdateClauseRenaming(7, 8);
  renamed = eliminator.RenameDeletion(original);
  REQUIRE(renamed.GetIndex() == 1);
  REQUIRE(renamed.GetClauseIndices() == vector<int>{5, 8, 4});
}
