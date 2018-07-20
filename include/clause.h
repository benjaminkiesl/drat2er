#ifndef DRAT2ER_CLAUSE_H_
#define DRAT2ER_CLAUSE_H_

#include <vector>
#include <string>

namespace drat2er
{

class Clause
{
  friend void swap(Clause&, Clause&);

 public:
  Clause();
  Clause(const Clause& other);
  Clause(Clause&& other);
  Clause& operator=(Clause other);

  void AddLiteral(int literal);
  bool ContainsLiteral(int literal) const;
  operator std::string() const;

  auto size() const{
    return literals_.size();
  }

  auto begin() const {
    return literals_.begin();
  }

  auto end() const {
    return literals_.end();
  }

  auto& operator[] (size_t index) {
    return literals_[index];
  }

 private:
  std::vector<int> literals_;
};

} // namespace

#endif
