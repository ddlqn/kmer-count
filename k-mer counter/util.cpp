//
//  tools.cpp
//  k-mer counter
//
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
#include "util.hpp"

// Encodes a k-mer as a value of base 5. This ensures no collisions up to k = 27
// and makes the hash reversible.
unsigned long long ComputeHash(const char * kmer, int k) {
  unsigned long long hash = 0;
  for (int i = 0; i < k; ++i) {
    hash *= 5;
    hash += BaseToIndex(kmer[i]);
  }
  return hash;
}

// Creates a string from an encoded kmer
std::string ComputeKmerFromHash(unsigned long long hash, int k) {
  std::string kmer;
  for (int i = 0; i < k; i++) {
    kmer.push_back(IndexToBase(hash%5));
    hash /= 5;
  }
  std::reverse(kmer.begin(), kmer.end());
  return kmer;
}

int BaseToIndex(char c) {
  switch (c) {
    case 'A':
    case 'a':
      return 0;
    case 'C':
    case 'c':
      return 1;
    case 'G':
    case 'g':
      return 2;
    case 'T':
    case 't':
      return 3;
    case 'N':
    case 'n':
      return 4;
    default:
      throw "unexpected character in sequence!";
  }
}

char IndexToBase(int i) {
  switch (i) {
    case 0:
      return 'A';
    case 1:
      return 'C';
    case 2:
      return 'G';
    case 3:
      return 'T';
    case 4:
      return 'N';
    default:
      throw "invalid index";
  }
}

bool operator>(const KmerResult& r1, const KmerResult& r2) {
  return r1.second > r2.second ||
  (r1.second == r2.second && r1.first < r2.first);
}


