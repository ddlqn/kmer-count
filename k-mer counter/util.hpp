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
#include <set>

struct KmerResultCompare;

typedef std::pair<std::string, unsigned long long> KmerResult;
typedef std::set<KmerResult, KmerResultCompare> KmerResultSet;

struct KmerResultCompare {
  bool operator()(const KmerResult& r1,
                  const KmerResult& r2) {
    return r1.second > r2.second ||
    (r1.second == r2.second && r1.first < r2.first);
  }
};

bool operator>(const KmerResult& r1, const KmerResult& r2);

unsigned long long ComputeHash(char * kmer, long k);
std::string ComputeKmerFromHash(unsigned long long hash, long k);
int BaseToIndex(char c);
char IndexToBase(int i);

#endif /* tools_hpp */
