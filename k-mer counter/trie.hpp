//
//  trie.hpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 23/03/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#ifndef trie_hpp
#define trie_hpp

#include <stdio.h>
#include <array>
#include <string>
#include <tuple>
#include <set>
#include <boost/circular_buffer.hpp>
#include "buffer.hpp"
#include <mutex>

struct KmerResultCompare;

class KmerTrie {
public:
  typedef std::tuple<std::string, unsigned int> KmerResult;
  
  KmerTrie();
  ~KmerTrie();
  void InsertKmer(const std::string &kmer, int pos = 0);
  void InsertKmer(CircularBufferIterator begin, CircularBufferIterator end);
  void InsertKmer(boost::circular_buffer<char>::iterator begin,
                  boost::circular_buffer<char>::iterator end);
  void InsertKmer(const char * buf, long k, int pos = 0);
  std::set<KmerResult, KmerResultCompare> GetTopKmers(long long int n, long k);
  std::set<KmerResult, KmerResultCompare> GetAllKmers(long k);
private:
  typedef std::array<KmerTrie *,5> ChildArray;

  // instance variables
  int count;
  std::mutex lock;
  ChildArray children;
  
  // private methods
  static int BaseToIndex(char c);
  static char IndexToBase(int i);
  void GetTopKmers(long long int n, long k, std::set<KmerResult, KmerResultCompare> &result_set, std::string &prefix);
};

struct KmerResultCompare {
  bool operator()(const KmerTrie::KmerResult& r1, const KmerTrie::KmerResult& r2) {
    return std::get<1>(r1) > std::get<1>(r2) ||
    (std::get<1>(r1) == std::get<1>(r2) && std::get<0>(r1) < std::get<0>(r2));
  }
};

bool operator>(const KmerTrie::KmerResult& r1, const KmerTrie::KmerResult& r2);

#endif /* trie_hpp */
