//
//  KmerCounter.hpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 31/03/2016.
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
  const long buffer_size = 1000000;
  char * buffer;
  std::ifstream in_file;
  KmerResultSet result_set;

  void FillKmerTrie(unsigned long k, KmerTrie &trie);
  void ComputeTopKmersUsingTrie(unsigned long n, unsigned long k);
  void ComputeTopKmersUsingFiles(unsigned long n, unsigned long k);
  void FillBuffer(long offset);
  void WriteKmersToTempFiles(unsigned long k);
public:
  KmerCounter() {}
  KmerCounter(std::string file_name);
  ~KmerCounter();
  KmerResultSet GetTopKmers(unsigned long top_N, unsigned long k);
  void reset();
};

#endif /* KmerCounter_hpp */
