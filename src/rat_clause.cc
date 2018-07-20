#include "rat_clause.h"
#include <vector>
#include <memory>
#include <iostream>
#include "clause.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::cout;
using std::endl;

namespace drat2er
{

ResolutionPartner::ResolutionPartner(int index) : index_ {index} {}

int ResolutionPartner::GetIndex() const
{
  return index_;
}

vector<int>& ResolutionPartner::Hints()
{
  return hints_;
}

RatClause::RatClause(int index) : index_(index)
{
  clause_ = make_unique<Clause>();
}

int RatClause::GetIndex() const
{
  return index_;
}

const shared_ptr<Clause> RatClause::GetClause() const
{
  return clause_;
}

void RatClause::AddLiteral(int literal)
{
  clause_->AddLiteral(literal);
}

int RatClause::GetPivot() const
{
  if(clause_->size() != 0) {
    return *clause_->begin();
  }
  return 0;
}

const vector<ResolutionPartner>& RatClause::GetResolutionPartners() const
{
  return resolution_partners_;
}

void RatClause::AddResolutionPartner(ResolutionPartner resolution_partner)
{
  resolution_partners_.emplace_back(resolution_partner);
}

void RatClause::Print() const {
  cout << "RAT " << GetIndex() << " = '";
  for(auto literal : *GetClause()){
    cout << literal << " ";
  } 
  cout << "' with partners: ";
  for(auto partner : GetResolutionPartners()){
    cout << partner.GetIndex() << " (";
    for(auto hint : partner.Hints()){
      cout << hint << " ";
    }
    cout << ")" << ", ";
  }
  cout << endl;
}

} // namespace
