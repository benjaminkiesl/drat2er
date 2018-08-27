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

class ProofTransformer : public LratParserObserver
{
 public:
  ProofTransformer(const std::string& verbose_message, 
                   bool print_progress);
  void Transform(const std::string& input_file, 
                 std::ostream& output_stream);
  void Transform(const std::string& input_file, 
                 const std::string& output_file);
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
  void WriteClausesToOutput(const std::vector<Clause>& clauses);
  void WriteClauseToOutput(const Clause& clause);
  void PrintVerboseMessage() const;
  void InitProgressBar(const std::string& input_file);
  void PrintProgress(bool increment_number_of_lines_processed=true);

  bool is_output_lrat_;

 private:

  std::ostream* output_stream_;
  std::string verbose_message_;
  bool print_progress_;
  int number_of_lines_processed_;
  ProgressBar progress_bar_;
};

} // namespace

#endif