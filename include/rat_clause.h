#ifndef DRAT2ER_RAT_CLAUSE_H_
#define DRAT2ER_RAT_CLAUSE_H_

#include <vector>
#include <memory>

namespace drat2er
{

class Clause;

class ResolutionPartner
{
 public:
  ResolutionPartner(int index);
  int GetIndex() const;
  std::vector<int>& Hints();

 private:
  int index_;
  std::vector<int> hints_;
};

class RatClause
{
 public:
  RatClause(int index);
  int GetIndex() const;
  const std::shared_ptr<Clause> GetClause() const;
  int GetPivot() const;
  void AddLiteral(int literal);
  const std::vector<ResolutionPartner>& GetResolutionPartners() const;
  void AddResolutionPartner(ResolutionPartner resolution_partner);
  void Print() const;
 private:
  int index_;
  std::shared_ptr<Clause> clause_;
  std::vector<ResolutionPartner> resolution_partners_;
};

} // namespace

#endif
