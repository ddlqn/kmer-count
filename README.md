# kmer-count

This program reads a DNA sequence from an input file and outputs the N most
common k-mers. Due to the internal representation of the k-mers (detailed
below), it only works for k <= 29. The following assumptions are made:
  - Input files contain a single valid DNA sequence (alphabet {A, C, G, T, N})
  - The sequence is on a single line, without whitespace (except for a possible
    newline character at the end of the file).

Implementation details
----------------------

In order to strike a balance between speed an memory requirements, two
different approaches are used, depending on k:

  - For k <= 10, the number of possible k-mers is small enough to fit into
    memory. Therefore, the program uses a trie structure (prefix tree) to
    store and count the k-mers. Once the input sequence has been read, the
    counts are extracted from the leaves.

  - For 10 < k <= 29, the k-mers are binned into files according to their first
    two bases. Bases 3 to k are encoded as numbers of base 5. Up to 27 bases
    can be stored as a unique 64-bit number in this way, leading to the maximum
    k of 29. The disk space required is therefore about 8 times the size of the
    input file (e.g. with a 1GB input file, 8GB of disk space are necessary).
    In a second stage, the files are read back into memory one by one, and the
    k-mers are counted.

In both cases, only the N most common k-mers are kept during the counting
phase. If there are multiple k-mers with the same count, they are ordered
lexicographically. In case the N-th and (N+1)-th k-mers have the same count,
only the N-th k-mer is reported.


Building
--------

The source code can be compiled using the included makefile:

    cd k-mer_counter
    make depend
    make

will compile the executable.

The program was validated with a set of tests (using the [Catch framework][1]).
To run these tests, use

    make depend
    make test
    ./test

[1]: https://github.com/philsquared/Catch
