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

#include "proof_reverser.h"
#include "instruction_serialization.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "clause.h"
#include "deletion.h"

using std::endl;

namespace drat2er
{

ProofReverser::ProofReverser(const bool print_progress)
         :  ProofTransformer("Reversing order of clauses in the proof.", 
                             print_progress,
                             true /* parse the file backwards */)
{
}

void ProofReverser::HandleProperRatAddition(const RatClause& rat)
{
  OutputStream() << ToLRAT(rat) << endl;
}

void ProofReverser::HandleRupAddition(const RupClause& rup)
{
  OutputStream() << ToLRAT(rup) << endl;
}

void ProofReverser::HandleDeletion(const Deletion& deletion)
{
  OutputStream() << ToLRAT(deletion) << endl;
}

void ProofReverser::HandleExtension(const Clause& extension)
{
  OutputStream() << ToLRATExtension(extension) << endl;
}

} // namespace drat2er
