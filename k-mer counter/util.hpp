//
//  tools.hpp
//  k-mer counter
//
//  A number of utility functions and definitions
//  
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#ifndef util_hpp
#define util_hpp

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

unsigned long long ComputeHash(const char * kmer, int k);
std::string ComputeKmerFromHash(unsigned long long hash, int k);

int BaseToIndex(char c);
char IndexToBase(int i);

inline unsigned long long GetFileLength(std::ifstream &file) {
  std::streampos cur_pos = file.tellg();
  file.seekg(-1, file.end);
  unsigned long long file_length = file.tellg();
  if (file.peek() != '\n') {
    ++file_length;
  }
  file.seekg(cur_pos);
  return file_length;
}


#endif /* util_hpp */
