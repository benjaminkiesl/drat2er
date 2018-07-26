#ifndef DRAT2ER_LRAT_PARSER_H_
#define DRAT2ER_LRAT_PARSER_H_

#include <string>
#include <fstream>
#include <vector>
#include <memory>

namespace drat2er
{

class Clause;
class RupClause;
class RatClause;
class Deletion;

class LratParserObserver
{
 public:
  virtual void HandleDeletion(const Deletion& deletion) = 0;
  virtual void HandleProperRatAddition(const RatClause& rat) = 0;
  virtual void HandleRupAddition(const RupClause& rup) = 0;
  virtual void HandleComment(const std::string& comment_line) = 0;
};

class LratParser
{
 public:
  void ParseFile(const std::string& proof_file_path);
  void RegisterObserver(std::shared_ptr<LratParserObserver> observer);
  static bool IsProperRatAddition(const std::string& proof_line);
  static bool IsDeletion(const std::string& proof_line);
  static bool IsComment(const std::string& proof_line);
  static Deletion ParseDeletion(const std::string& proof_line);
  static RupClause ParseRup(const std::string& proof_line);
  static RatClause ParseProperRat(const std::string& proof_line);
  static bool ContainsNoLiterals(const std::string& proof_line);
 private:
  static void ParseClausePart(Clause& clause, std::stringstream& line_stream);

  std::shared_ptr<LratParserObserver> observer_;
};

} // namespace

#endif
