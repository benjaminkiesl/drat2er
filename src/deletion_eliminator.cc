#include "deletion_eliminator.h"
#include "clause.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "deletion.h"

namespace drat2er
{

void HandleDeletion(const Deletion& deletion){
  // Do nothing, i.e., do not write the deletion to the output.
}

void DeletionEliminator::HandleProperRatAddition(const RatClause& rat){
  WriteClauseToOutput(rat);
}

void DeletionEliminator::HandleExtension(const Clause& extension){
  WriteClauseToOutput(extension);
}

void DeletionEliminator::HandleRupAddition(const RupClause& rup){
  WriteClauseToOutput(rup);
}

void DeletionEliminator::HandleComment(const std::string& comment_line){
  OutputStream() << comment_line << std::endl;
}

} // namespace
