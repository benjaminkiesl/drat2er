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

void DeletionEliminator::ObserveDeletion(const Deletion& deletion){
  // do nothing
} 

void DeletionEliminator::ObserveProperRatAddition(const RatClause& rat){
  output_stream_ << rat.ToLrat() << endl;
}

void DeletionEliminator::ObserveRupAddition(const RupClause& rup){
  output_stream_ << rup.ToLrat() << endl;
}

void DeletionEliminator::ObserveComment(const string& comment_line){
  output_stream_ << comment_line << endl;
}

} // namespace
