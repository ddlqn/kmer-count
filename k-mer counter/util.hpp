//
//  tools.hpp
//  k-mer counter
//
//  
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#ifndef tools_hpp
#define tools_hpp

#include <fstream>
#include <map>
#include <boost/container/flat_set.hpp>
#include "trie.hpp"

struct KmerPairCompare;

typedef std::pair<long long, long long> KmerPair;
//typedef boost::container::flat_set<KmerPair, KmerPairCompare> KmerSet;
//typedef std::set<KmerPair, KmerPairCompare> KmerSet;

int FillKmerTrie(std::ifstream &in_file, long k, KmerTrie &trie);
int FillKmerTrie_boost(std::ifstream &in_file, long k, KmerTrie &trie);
int FillKmerTrie_parallel(std::ifstream &in_file, long k, KmerTrie &trie);
int FillKmerTrieFromBuffer(char * buffer, long k, long start, long end, KmerTrie &trie);

int FillHashTable(std::ifstream &in_file, std::map<unsigned long long, unsigned long long> &kmer_map, long k);
unsigned long long ComputeHash(char * kmer, long k);
int BaseToIndex(char c);
char IndexToBase(int i);
KmerTrie::KmerResultSet GetTopKmersFromMap(std::map<unsigned long long, unsigned long long> &kmer_map, long k, unsigned long long n);
KmerTrie::KmerResultSet GetTopKmersFromFile(std::ifstream &in_file, long n, long k);

struct KmerPairCompare {
  bool operator()(const KmerPair& p1,
                  const KmerPair& p2) const {
    return p1.first < p2.first;
  }
};

#endif /* tools_hpp */
