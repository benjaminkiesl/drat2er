# drat2er: Proof Transformer for Propositional Logic

drat2er is a tool for transforming proofs that are usually produced by [SAT solvers](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem). It takes as input a propositional formula (specified in the [DIMACS format](http://www.satcompetition.org/2009/format-benchmarks2009.html)) together with a [DRAT](https://arxiv.org/pdf/1610.06229.pdf) proof (DRAT is the current standard format for proofs in SAT solving), and outputs an extended-resolution proof of the formula in either the [TRACECHECK](http://fmv.jku.at/tracecheck/README.tracecheck) or the DRAT format. The details of this proof transformation are described in the paper "[Extended Resolution Simulates DRAT](http://www.cs.utexas.edu/users/marijn/publications/ijcar18.pdf)" (IJCAR 2018). 

## Getting Started

### Prerequisites

To build drat2er, you need to have [CMake](https://cmake.org/) and [Make](https://www.gnu.org/software/make/) installed.

### Installation

The easiest way to build drat2er is to just execute the script 'build.sh' in the main directory. After this, the executable 'drat2er' is located at 'build/bin/drat2er'.

The build.sh script simply creates a directory 'build' within the main directory and then executes 'cmake ..' followed by 'make' from within that build directory. If you don't want to use Make, and are familiar with CMake, you can also build the project for any platform you want using the CMake file CMakeLists.txt.

### Running drat2er

To run drat2er, just execute the following command from the shell: 

`./drat2er INPUT_FORMULA_PATH INPUT_PROOF_PATH [OUTPUT_PROOF_PATH]`

* `INPUT_FORMULA_PATH` is the path to a DIMACS file containing a propositional formula.
* `INPUT_PROOF_PATH` is the path to a DRAT file containing a DRAT proof of the formula.
* `OUTPUT_PROOF_PATH` is the path to the resulting extended-resolution proof. If `OUTPUT_PROOF_PATH` is not specified, the proof is written to the standard output.

For example, if your formula is in the DIMACS file 'formula.cnf' (located in the directory from which you call drat2er) and your DRAT proof is in the file 'proof.drat', then the following command writes its output to the file 'proof.er':

`./drat2er formula.cnf proof.drat proof.er`

Further arguments can be passed. For details call `./drat2er --help`.
You might want to pass the parameter `-v` because it tells drat2er to print a progress bar. For example:

`./drat2er formula.cnf proof.drat -v proof.er`

Finally, make sure you have write access to the directory from which you call drat2er because drat2er produces temporary files in that directory.

### Output Format

By default, drat2er outputs proofs in the so-called [TRACECHECK](http://fmv.jku.at/tracecheck/README.tracecheck) format. The TRACECHECK format allows for a compact representation of resolution proofs and extended-resolution proofs. There exists also a proof checker for TRACECHECK proofs that can be downloaded [here](http://fmv.jku.at/tracecheck/). A TRACECHECK proof does not contain a separate line for every single resolvent but instead combines several resolution steps into a single "resolution chain".

For instance, if the clause `1 | 2` (with 1 and 2 being literals) is obtained by first resolving the clause `1 | 3` with the clause `-3 | 4` and then resolving the resulting clause (`1 | 4`) with the clause `-4 | 2`, a TRACECHECK proof might encode this as one statement that basically says "the clause `1 | 2` is derived via a resolution chain from the clauses `1 | 3`, `-3 | 4`, and `-4 | 2`", without mentioning the intermediate resolvent `1 | 4`. If the clauses `1 | 3`, `-3 | 4`, and `-4 | 2` are contained in the original formula, then this derivation can be represented in a TRACECHECK proof as follows:

	1 1 3 0 0
	2 -3 4 0 0
	3 -4 2 0 0
	4 1 2 0 1 2 3 0

The first number of a statement indicates the index of the clause (for instance, the clause `1 | 3` above has the index 1). The clause index is followed by a list of literals contained in the clause. This list ends with a 0. After the first 0, the indices of the clauses used for deriving the clause via a resolution chain are listed (the clause `1 | 2` above is derived via a resolution chain from the clauses with the indices 1, 2, and 3). If a clause is contained in the original formula or if it is a definition clause (in an extended-resolution proof) then this list of indices is empty. Finally, a second 0 indicates the end of a statement.

By passing the parameter `-f drat` (or, alternatively, `--format=drat`) to drat2er, it outputs the extended-resolution proof in the [DRAT](https://arxiv.org/pdf/1610.06229.pdf) format. This might seem confusing at first, since the whole purpose of drat2er is to transform DRAT proofs into another form. The explanation is as follows: The resulting proof is in fact an extended-resolution proof, meaning that all clauses of the proof are either definition clauses or can be derived from previous clauses via resolution chains. It is only the output format that is still DRAT. The resulting proof can thus be checked with a DRAT proof checker.

## Built With

* [drat-trim](https://github.com/marijnheule/drat-trim) - Proof checker for DRAT proofs.
* [Catch2](https://github.com/catchorg/Catch2) - Framework for unit testing.
* [CLI11](https://github.com/CLIUtils/CLI11) - Command line parser for C++11.
