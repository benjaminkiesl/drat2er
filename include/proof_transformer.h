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

#ifndef DRAT2ER_PROOF_TRANSFORMER_H_
#define DRAT2ER_PROOF_TRANSFORMER_H_

#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include "lrat_parser.h"
#include "progress_bar.h"

namespace drat2er
{

class Clause;
class RatClause;
class RupClause;
class Deletion;

// A ProofTransformer object can be used to transform LRAT proofs.
// A ProofTransformer creates its own LratParser object and acts itself as the
// observer of this LratParser. It contains additional logic to print the
// progress, etc. This class acts as an abstract superclass for concrete
// ProofTransformers.
class ProofTransformer : public LratParserObserver
{
 public:
  // Creates a ProofTransformer. The string parameter 'verbose_message' is
  // printed when the Transform function is called. The parameter 
  // 'print_progress' indicates if a progress bar should print the current
  // progress of the proof transformation.
  ProofTransformer(const std::string& verbose_message, 
                   const bool print_progress,
                   const bool is_parse_backwards = false);

  // Takes a path to an input file and transforms this input file, thereby
  // writing the result to the given output stream.
  void Transform(const std::string& input_file, 
                 std::ostream& output_stream);

  // Takes a path to an input file and transforms this input file, thereby
  // writing the result to the given output file.
  void Transform(const std::string& input_file, 
                 const std::string& output_file);

  // LratParserObserver callbacks:
  virtual void ObserveProperRatAddition(const RatClause& rat) final override;
  virtual void ObserveRupAddition(const RupClause& rup) final override;
  virtual void ObserveDeletion(const Deletion& deletion) final override;
  virtual void ObserveComment(const std::string& comment_line) final override;
  virtual void ObserveExtension(const Clause& extension) final override;

 protected:
  virtual void HandleProperRatAddition(const RatClause& rat);
  virtual void HandleRupAddition(const RupClause& rup);
  virtual void HandleDeletion(const Deletion& deletion);
  virtual void HandleComment(const std::string& comment_line);
  virtual void HandleExtension(const Clause& extension);
  std::ostream& OutputStream();
  void PrintVerboseMessage() const;
  void InitProgressBar(const std::string& input_file);
  void PrintProgress(bool increment_number_of_lines_processed=true);

 private:

  std::ostream* output_stream_;
  std::string verbose_message_;
  bool print_progress_;
  bool is_parse_backwards_;
  int number_of_lines_processed_;
  ProgressBar progress_bar_;
};

} // namespace drat2er

#endif
