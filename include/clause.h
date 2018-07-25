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
  friend bool operator==(const Clause& lhs, const Clause& rhs);

 public:
  Clause();
  Clause(std::initializer_list<int> literals);
  Clause(const Clause& other) = default;
  Clause(Clause&& other);
  Clause& operator=(Clause other);
  
  int GetIndex() const;
  void SetIndex(int index);
  const std::vector<int>& GetLiterals() const;
  void SetLiterals(const std::vector<int>& literals);
  void AddLiteral(int literal);
  int GetMaxVariable() const;
  bool ContainsLiteral(int literal) const;
  std::string ToDimacs() const;
  virtual std::string ToLrat() const;

  auto size() const { return literals_.size(); }
  auto empty() const { return literals_.empty(); }
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
