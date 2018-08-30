# drat2er: Proof Transformer for Propositional Logic

drat2er is a tool for transforming proofs that are usually produced by [SAT solvers](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem). It takes as input a propositional formula (specified in the [DIMCAS format](http://www.satcompetition.org/2009/format-benchmarks2009.html)) together with a [DRAT](https://arxiv.org/pdf/1610.06229.pdf) proof (DRAT is the current standard format for proofs in SAT solving), and outputs an extended-resolution proof of the formula in either the [TRACECHECK](http://fmv.jku.at/tracecheck/README.tracecheck) or the DRAT format. The details of this proof transformation are described in the paper "[Extended Resolution Simulates DRAT](http://www.cs.utexas.edu/users/marijn/publications/ijcar18.pdf)" (IJCAR 2018). 

## Getting Started

### Prerequisites

To build drat2er, you need to have [CMake](https://cmake.org/) and [Make](https://www.gnu.org/software/make/) installed.

### Installation

The easiest way to build drat2er is to just execute the script 'build.sh' in the main directory (make sure you have the rights to execute the script). After this, the executable 'drat2er' is located at 'build/bin/drat2er'.

The build.sh script simply creates a directory 'build' within the main directory and then executes 'cmake ..' followed by 'make' from within the build directory. If you don't want to use Make, and are familiar with CMake, you can also build the project for any platform you want using the CMake file CMakeLists.txt.

### Running drat2er

To run drat2er, just execute the following command from the shell: 

`./drat2er INPUT_FORMULA_PATH INPUT_PROOF_PATH [OUTPUT_PROOF_PATH]`

* `INPUT_FORMULA_PATH` is the path of a DIMACS file containing a propositional formula.
* `INPUT_PROOF_PATH` is the path of a DRAT file containing a DRAT proof of the formula.
* `OUTPUT_PROOF_PATH` is the path of the resulting extended-resolution proof. If `OUTPUT_PROOF_PATH` is not specified, the proof is written to the standard output.

For example, if your formula is in the DIMACS file 'formula.cnf' (located in the directory from which you call drat2er) and your DRAT proof is in the file 'proof.drat', then the following command writes its output to the file 'proof.er':

`./drat2er formula.cnf proof.cnf proof.er`

Further arguments can be passed. For details call `./drat2er --help`.
You might want to pass the parameter '-v' because it prints a progress bar to the standard output. For example:

`./drat2er formula.cnf proof.cnf -v proof.er`

Finally, make sure you have write access to the directory from which you call drat2er because drat2er produces temporary files in that directory.

## Built With

* [drat-trim](https://github.com/marijnheule/drat-trim) - Used for performing one step of the transformation.
* [Catch2](https://github.com/catchorg/Catch2) - Framework for unit testing.
* [CLI11](https://github.com/CLIUtils/CLI11) - Command line parser for C++11.
