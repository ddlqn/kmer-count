//
//  trie.hpp
//  k-mer counter
//
//  Trie data structure to store and count k-mers. Each node has a counter (only
//  used in the leaves) and pointers to the child nodes, each representing one
//  letter of the alphabet (A, C, G, T, N). Child nodes are allocated when a new
//  k-mer is inserted that requires them. This data structure is only used if
//  the number of possible k-mers can fit into memory (k <= 10).
//
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#ifndef trie_hpp
#define trie_hpp

#include <stdio.h>
#include <array>
#include <string>
#include <set>
#include "util.hpp"

class KmerTrie {
public:
  KmerTrie();
  ~KmerTrie();
  void InsertKmer(const std::string &kmer, int pos = 0);
  void InsertKmer(const char * buf, int k, int pos = 0);
  KmerResultSet GetTopKmers(unsigned int n, int k);
  KmerResultSet GetAllKmers(int k);
private:
  int count;
  std::array<KmerTrie *,5> children;
  
  void GetTopKmers(long long int n, int k, KmerResultSet &result_set,
                   std::string &prefix);
};

#endif /* trie_hpp */
