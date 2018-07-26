#include "catch.hpp"
#include <vector>
#include "rat_eliminator.h"
#include "rat_clause.h"
#include "rup_clause.h"

using namespace drat2er;
using std::vector;

TEST_CASE("RatEliminator::FirstDefinitionClause Ternary RAT"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  const int pivot = 1;
  const int new_variable = 4;
  RatClause rat{pivot, 2, 3};
  Clause expected{new_variable, 2, 3};
  
  auto definition = eliminator.FirstDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::FirstDefinitionClause Ternary RAT, Negated Pivot"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  const int pivot = -1;
  const int new_variable = 4;
  RatClause rat{pivot, 2, 3};
  Clause expected{new_variable, 2, 3};
  
  auto definition = eliminator.FirstDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::FirstDefinitionClause Unit RAT"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  const int pivot = 1;
  const int new_variable = 2;
  RatClause rat{pivot};
  Clause expected{2};
  
  auto definition = eliminator.FirstDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::FirstDefinitionClause Unit RAT, Negated Pivot"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  const int pivot = -1;
  const int new_variable = 2;
  RatClause rat{pivot};
  Clause expected{2};
  
  auto definition = eliminator.FirstDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::SecondDefinitionClause Ternary RAT"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  const int pivot = 1;
  const int new_variable = 4;
  RatClause rat{pivot, 2, 3};
  Clause expected{new_variable, -pivot};
  
  auto definition = eliminator.SecondDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::SecondDefinitionClause Ternary RAT, Negated Pivot"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  const int pivot = -1;
  const int new_variable = 4;
  RatClause rat{pivot, 2, 3};
  Clause expected{new_variable, -pivot};
  
  auto definition = eliminator.SecondDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::SecondDefinitionClause Unit RAT"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  const int pivot = 1;
  const int new_variable = 2;
  RatClause rat{pivot};
  Clause expected{new_variable, -pivot};
  
  auto definition = eliminator.SecondDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::SecondDefinitionClause Unit RAT, Negated Pivot"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  const int pivot = -1;
  const int new_variable = 2;
  RatClause rat{pivot};
  Clause expected{new_variable, -pivot};
  
  auto definition = eliminator.SecondDefinitionClause(rat, new_variable);
  REQUIRE(definition.GetLiterals() == expected.GetLiterals());
}

TEST_CASE("RatEliminator::ThirdBlockOfDefinitionClauses Ternary RAT"){
  RatEliminator eliminator{"", nullptr, 0, 0};
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
  RatEliminator eliminator{"", nullptr, 0, 0};
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
  RatEliminator eliminator{"", nullptr, 0, 0};
  const int pivot = 1;
  const int new_variable = 2;
  RatClause rat{pivot};
  
  REQUIRE(eliminator.ThirdBlockOfDefinitionClauses(rat, new_variable).empty());
}

TEST_CASE("RatEliminator::ThirdBlockOfDefinitionClauses Unit RAT"
          ", Negated Pivot"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  const int pivot = -1;
  const int new_variable = 2;
  RatClause rat{pivot};
  
  REQUIRE(eliminator.ThirdBlockOfDefinitionClauses(rat, new_variable).empty());
}

TEST_CASE("RatEliminator::CorrespondingDefinition Ternary RAT"){
  RatEliminator eliminator{"", nullptr, 0, 0};
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
  RatEliminator eliminator{"", nullptr, 0, 0};
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
  RatEliminator eliminator{"", nullptr, 0, 0};
  REQUIRE(1 == eliminator.RenameLiteral(1));
  eliminator.UpdateRenaming(1, 2);
  REQUIRE(2 == eliminator.RenameLiteral(1));
}

TEST_CASE("RatEliminator::RenameLiteral - Double renaming"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  REQUIRE(1 == eliminator.RenameLiteral(1));
  eliminator.UpdateRenaming(1, 2);
  eliminator.UpdateRenaming(2, 3);
  REQUIRE(3 == eliminator.RenameLiteral(1));
}

TEST_CASE("RatEliminator::RenameLiteral - Quadruple renaming"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  REQUIRE(1 == eliminator.RenameLiteral(1));
  eliminator.UpdateRenaming(1, 2);
  eliminator.UpdateRenaming(2, 3);
  eliminator.UpdateRenaming(3, 4);
  eliminator.UpdateRenaming(4, 5);
  REQUIRE(5 == eliminator.RenameLiteral(1));
}

TEST_CASE("RatEliminator::RenameClause - Clause"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  Clause original{1, 2, 3};
  REQUIRE(eliminator.RenameClause(original).GetLiterals() == 
      original.GetLiterals()); 
  eliminator.UpdateRenaming(1, 4);
  eliminator.UpdateRenaming(4, 5);
  eliminator.UpdateRenaming(5, 6);
  eliminator.UpdateRenaming(2, 7);
  REQUIRE(eliminator.RenameClause(original).GetLiterals() == 
      vector<int>{6, 7, 3});
}

TEST_CASE("RatEliminator::RenameClause - RupClause"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  RupClause original{1, 2, 3};
  REQUIRE(eliminator.RenameClause(original).GetLiterals() == 
      original.GetLiterals()); 
  eliminator.UpdateRenaming(1, 4);
  eliminator.UpdateRenaming(4, 5);
  eliminator.UpdateRenaming(5, 6);
  eliminator.UpdateRenaming(2, 7);
  REQUIRE(eliminator.RenameClause(original).GetLiterals() == 
      vector<int>{6, 7, 3});
}

TEST_CASE("RatEliminator::RenameClause - RatClause"){
  RatEliminator eliminator{"", nullptr, 0, 0};
  RatClause original{1, 2, 3};
  REQUIRE(eliminator.RenameClause(original).GetLiterals() == 
      original.GetLiterals()); 
  eliminator.UpdateRenaming(1, 4);
  eliminator.UpdateRenaming(4, 5);
  eliminator.UpdateRenaming(5, 6);
  eliminator.UpdateRenaming(2, 7);
  REQUIRE(eliminator.RenameClause(original).GetLiterals() == 
      vector<int>{6, 7, 3});
}

