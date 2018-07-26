#include "deletion_eliminator.h"
#include <vector>
#include <fstream>
#include "clause.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "lrat_parser.h"
#include "deletion.h"

using std::string;
using std::vector;
using std::endl;

namespace drat2er
{

DeletionEliminator::DeletionEliminator(std::string output_file) 
                                     : output_stream_{output_file}{}

void DeletionEliminator::HandleDeletion(const Deletion& deletion){
  // do nothing
} 

void DeletionEliminator::HandleProperRatAddition(const RatClause& rat){
  output_stream_ << rat.ToDimacs() << endl;
}

void DeletionEliminator::HandleRupAddition(const RupClause& rup){
  output_stream_ << rup.ToDimacs() << endl;
}

void DeletionEliminator::HandleComment(const string& comment_line){
  output_stream_ << comment_line << endl;
}

} // namespace
