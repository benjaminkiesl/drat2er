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

#include "proof_transformer.h"
#include <cassert>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include "rat_clause.h"
#include "rup_clause.h"
#include "clause.h"
#include "deletion.h"
#include "file_helper.h"

using std::string;
using std::cout;
using std::endl;
using std::ofstream;

namespace drat2er
{

ProofTransformer::ProofTransformer(const std::string& verbose_message,
                                   const bool print_progress,
                                   const bool is_parse_backwards)
                                    : output_stream_{nullptr},
                                      verbose_message_{verbose_message},
                                      print_progress_{print_progress},
                                      is_parse_backwards_{is_parse_backwards},
                                      number_of_lines_processed_{0},
                                      progress_bar_{}
{
}

void ProofTransformer::Transform(const std::string& input_file,
                                 std::ostream& output_stream)
{
  PrintVerboseMessage();
  InitProgressBar(input_file);
  output_stream_ = &output_stream;
  LratParser lrat_parser;
  lrat_parser.RegisterObserver(this);
  lrat_parser.ParseFile(input_file, is_parse_backwards_);
}

void ProofTransformer::Transform(const string& input_file,
                                 const string& output_file)
{
  ofstream output_file_stream(output_file);
  Transform(input_file, output_file_stream);
}

void ProofTransformer::ObserveProperRatAddition(const RatClause& rat)
{
  HandleProperRatAddition(rat);
  PrintProgress();
}

void ProofTransformer::ObserveRupAddition(const RupClause& rup)
{
  HandleRupAddition(rup);
  PrintProgress();
}

void ProofTransformer::ObserveDeletion(const Deletion& deletion)
{
  HandleDeletion(deletion);
  PrintProgress();
}

void ProofTransformer::ObserveComment(const string& comment_line)
{
  HandleComment(comment_line);
  PrintProgress();
}

void ProofTransformer::ObserveExtension(const Clause& extension)
{
  HandleExtension(extension);
  PrintProgress();
}

std::ostream& ProofTransformer::OutputStream()
{
  assert(output_stream_ != nullptr);
  return *output_stream_;
}

void ProofTransformer::HandleProperRatAddition(const RatClause& rat)
{
  // Does nothing by default. Provide functionality in subclasses.
}

void ProofTransformer::HandleRupAddition(const RupClause& rup)
{
  // Does nothing by default. Provide functionality in subclasses.
}

void ProofTransformer::HandleDeletion(const Deletion& deletion)
{
  // Does nothing by default. Provide functionality in subclasses.
}

void ProofTransformer::HandleComment(const string& comment_line)
{
  // Does nothing by default. Provide functionality in subclasses.
}

void ProofTransformer::HandleExtension(const Clause& extension)
{
  // Does nothing by default. Provide functionality in subclasses.
}

void ProofTransformer::PrintVerboseMessage() const
{
  cout << "c drat2er: " << verbose_message_ << endl;
}

void ProofTransformer::InitProgressBar(const std::string& input_file)
{
  if(print_progress_) {
    progress_bar_.SetOverallNumberOfItems(1);
    PrintProgress(false);
    progress_bar_.SetOverallNumberOfItems(
        file_helper::GetNumberOfLines(input_file));
    number_of_lines_processed_ = 0;
  }
}

void ProofTransformer::PrintProgress(bool increment_number_of_lines_processed)
{
  if(print_progress_) {
    if(increment_number_of_lines_processed) {
      ++number_of_lines_processed_;
    }
    progress_bar_.PrintProgress(number_of_lines_processed_);
  }
}

} // namespace drat2er
