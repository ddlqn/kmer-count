//
//  trie.hpp
//  k-mer counter
//
//  Trie data structure to store and count k-mers. Each node has a counter (only
//  used in the leaves) and pointers to the child nodes, each representing one
//  letter of the alphabet (A, C, G, T, N). Child nodes are allocated when a new
//  k-mer is inserted that requires them.
//
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#ifndef trie_hpp
#define trie_hpp

#include <stdio.h>
#include <array>
#include <string>
#include <tuple>
#include <set>
#include "buffer.hpp"
#include "util.hpp"

class KmerTrie {
public:
  
  KmerTrie();
  ~KmerTrie();
  void InsertKmer(const std::string &kmer, int pos = 0);
  void InsertKmer(const char * buf, long k, int pos = 0);
  KmerResultSet GetTopKmers(long long int n, long k);
  KmerResultSet GetAllKmers(long k);
private:
  typedef std::array<KmerTrie *,5> ChildArray;

  // instance variables
  int count;
  ChildArray children;
  
  // private methods
  void GetTopKmers(long long int n, long k, KmerResultSet &result_set,
                   std::string &prefix);
};

#endif /* trie_hpp */
