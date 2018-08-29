// MIT License
//
// Copyright (c) 2018 Benjamin Kiesl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
// IN THE SOFTWARE.

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

  // Takes as input the path to an LRAT file and then parses the file.
  // For every occurrence of a certain object (like a deletion, RUP addition,
  // etc.) it calls its observer and passes the object to the observer, who
  // can in turn process it.
  void ParseFile(const std::string& proof_file_path);

  // Registers an observer of the LratParser. An observer is called for every
  // object (e.g., a deletion, RUP addition, etc.) that is encountered during
  // parsing of the LRAT proof.
  void RegisterObserver(LratParserObserver* observer);

  // Takes a line in an LRAT proof file and returns true if this line encodes
  // a proper RAT addition.
  static bool IsProperRatAddition(const std::string& proof_line);

  // Takes a line in an LRAT proof file and returns true if this line encodes
  // a deletion instruction.
  static bool IsDeletion(const std::string& proof_line);

  // Takes a line in an LRAT proof file and returns true if this line encodes
  // an extension/definition introduction.
  static bool IsExtension(const std::string& proof_line);

  // Takes a line in an LRAT proof file and returns true if this line encodes
  // a comment.
  static bool IsComment(const std::string& proof_line);

  // Parses a line of an LRAT proof that encodes a proper RAT addition and
  // returns a RatClause object that contains all the information of the
  // RAT addition.
  static RatClause ParseProperRat(const std::string& proof_line);

  // Parses a line of an LRAT proof that encodes a deletion instruction and
  // returns a Deletion object that contains all the information of the
  // deletion instruction.
  static Deletion ParseDeletion(const std::string& proof_line);

  // Parses a line of an LRAT proof that encodes an extension/definition
  // introduction and returns a Clause object that contains all the 
  // information of the definition clause.
  static Clause ParseExtension(const std::string& proof_line);

  // Parses a line of an LRAT proof that encodes a RUP addition and returns a 
  // RupClause object that contains all the  information of the RUP clause.
  static RupClause ParseRup(const std::string& proof_line);

  // Returns true if a given proof line contains no literals.
  static bool ContainsNoLiterals(const std::string& proof_line);

  // Takes a string that encodes an extension/definition introduction and
  // removes the 'e' symbol from it.
  static std::string RemoveE(const std::string& proof_line);

 private:
  static void ParseClausePart(Clause& clause, std::stringstream& line_stream);

  LratParserObserver* observer_;
};

} // namespace drat2er

#endif
