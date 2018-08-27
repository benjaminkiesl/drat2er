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

// LratParserObserver defines the interface for all classes that want to
// act as observers for the LratParser. These classes can register themselves
// for a specific LratParser and are then called whenever
// the parser encounters a deletion, proper RAT addition, RUP addition, 
// comment, or extension (definition introduction).
class LratParserObserver
{
 public:
  virtual void ObserveDeletion(const Deletion& deletion) = 0;
  virtual void ObserveProperRatAddition(const RatClause& rat) = 0;
  virtual void ObserveRupAddition(const RupClause& rup) = 0;
  virtual void ObserveComment(const std::string& comment_line) = 0;
  virtual void ObserveExtension(const Clause& definition_clause){};
};


// LratParser can parse input files in the LRAT format (including also
// extensions/definition introductions). It processes an LRAT file from
// beginning to end and, for every object it encounters (e.g., RAT additions,
// deletions, etc.), it calls its observer. An observer can register itself
// via the RegisterObserver member function.
// Usage: 1. Create an LratParser object.
//        2. Create an LratParserObserver object.
//        3. Register the LratParserObserver object by calling 
//           RegisterObserver on the LratParser object.
//        4. Call Parser::ParseFile() on the LratParser object.
class LratParser
{
 public:
  void ParseFile(const std::string& proof_file_path);
  void RegisterObserver(LratParserObserver* observer);
  static bool IsProperRatAddition(const std::string& proof_line);
  static bool IsDeletion(const std::string& proof_line);
  static bool IsExtension(const std::string& proof_line);
  static bool IsComment(const std::string& proof_line);
  static Deletion ParseDeletion(const std::string& proof_line);
  static RupClause ParseRup(const std::string& proof_line);
  static RatClause ParseProperRat(const std::string& proof_line);
  static Clause ParseExtension(const std::string& proof_line);
  static bool ContainsNoLiterals(const std::string& proof_line);
  static std::string RemoveE(const std::string& proof_line);
 private:
  static void ParseClausePart(Clause& clause, std::stringstream& line_stream);

  LratParserObserver* observer_;
};

} // namespace

#endif
