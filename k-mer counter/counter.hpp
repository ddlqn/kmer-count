//
//  KmerCounter.hpp
//  k-mer counter
//
//  Class encapsulating the counter functionality. A counter object is always
//  associated with an input file and can be used multiple times on the same
//  file.
//
//  Depending on the k-mer length (k), either a trie is used, or the k-mers from
//  the input file are hashed and then cached in temporary files. This reduces
//  the memory requirement at the expense of disk space (about 8x the input
//  size) and speed, but works for up to 27-mers.
//
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#ifndef KmerCounter_hpp
#define KmerCounter_hpp

#include <fstream>
#include <set>
#include <string>
#include <utility>
#include "trie.hpp"


class KmerCounter {
public:
  KmerCounter(std::string file_name);
  ~KmerCounter();
  KmerResultSet GetTopKmers(unsigned int top_N, int k);
private:
  static const long buffer_size = 1000000;
  // We create a file for every prefix of size 2:
  static const int number_of_files = 25;
  char * buffer;
  std::ifstream in_file;
  KmerResultSet result_set;
  
  void ComputeTopKmersUsingFiles(unsigned int top_N, int k);
  void ComputeTopKmersUsingTrie(unsigned int top_N, int k);
  void FillBuffer(long offset);
  void FillKmerTrie(int k, KmerTrie &trie);
  void ReadTempFile(int file_index, unsigned int top_N, int k);
  void reset();
  void WriteKmersToTempFiles(int k);
};

#endif /* KmerCounter_hpp */
