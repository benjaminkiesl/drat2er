#ifndef DRAT2ER_CLAUSE_H_
#define DRAT2ER_CLAUSE_H_

#include <vector>
#include <string>
#include <initializer_list>

namespace drat2er
{

class Clause
{
  friend void swap(Clause& lhs, Clause& rhs);

 public:
  Clause();
  Clause(const Clause& other) = default;
  Clause(Clause&& other);
  Clause& operator=(Clause other);

  int GetIndex() const;
  void SetIndex(int index);
  void AddLiteral(int literal);
  int GetMaxVariable() const;
  bool ContainsLiteral(int literal) const;
  operator std::string() const;
  auto size() const { return literals_.size(); }
  auto begin() const { return literals_.begin(); }
  auto end() const { return literals_.end(); }

 protected:
  std::vector<int> literals_;

 private:
  int index_;
  int max_variable_;
};

} // namespace

#endif
