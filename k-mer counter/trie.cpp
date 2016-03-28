//
//  trie.cpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 23/03/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include "trie.hpp"
#include <climits>
#include <iostream>


int KmerTrie::BaseToIndex(char c) {
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

char KmerTrie::IndexToBase(int i) {
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

KmerTrie::KmerTrie() {
  count = 0;
  for (auto &it : children) {
    it = NULL;
  }
}

KmerTrie::~KmerTrie() {
  for (auto &it : children) {
    if (it) {
      delete it;
    }
  }
}

void KmerTrie::InsertKmer(const std::string &kmer, int pos) {
  if (kmer.length() == pos) {
    count += 1;
  } else {
    int curIndex = BaseToIndex(kmer[pos]);
    if (!children[curIndex]) {
      children[curIndex] = new KmerTrie();
    }
    children[curIndex]->InsertKmer(kmer, ++pos);
  }
}

void KmerTrie::InsertKmer(CircularBufferIterator begin,
                          CircularBufferIterator end) {
  if (begin == end) {
    count += 1;
  } else {
    int curIndex = BaseToIndex(*begin);
    if (!children[curIndex]) {
      children[curIndex] = new KmerTrie();
    }
    children[curIndex]->InsertKmer(++begin, end);
  }
}

void KmerTrie::InsertKmer(boost::circular_buffer<char>::iterator begin,
                          boost::circular_buffer<char>::iterator end) {
  if (begin == end) {
    count += 1;
  } else {
    int curIndex = BaseToIndex(*begin);
    if (!children[curIndex]) {
      children[curIndex] = new KmerTrie();
    }
    children[curIndex]->InsertKmer(++begin, end);
  }
}

std::set<KmerTrie::KmerResult, KmerResultCompare> KmerTrie::GetTopKmers(long long int n, long k) {
  std::set<KmerResult, KmerResultCompare> result_set;
  std::string prefix;
  GetTopKmers(n, k, result_set, prefix);
  return result_set;
}

std::set<KmerTrie::KmerResult, KmerResultCompare> KmerTrie::GetAllKmers(long k) {
  std::set<KmerResult, KmerResultCompare> result_set;
  std::string prefix;
  GetTopKmers(LLONG_MAX, k, result_set, prefix);
  return result_set;
}

void KmerTrie::GetTopKmers(long long int n, long k,
                           std::set<KmerResult, KmerResultCompare> &result_set,
                           std::string &prefix) {
  if (prefix.length() == k) {
//    std::cout << prefix << std::endl;
    KmerResult r(prefix, count);
    if (result_set.size() < n) {
      result_set.insert(r);
    } else if (r > *(result_set.rbegin())) {
      result_set.erase(*(result_set.rbegin()));
      result_set.insert(r);
    }
  } else {
    for (int i = 0; i < 5; i++) {
      if (!children[i]) {
        continue;
      }
      prefix.push_back(IndexToBase(i));
      children[i]->GetTopKmers(n, k, result_set, prefix);
      prefix.pop_back();
    }
  }
}

bool operator>(const KmerTrie::KmerResult& r1, const KmerTrie::KmerResult& r2) {
  return std::get<1>(r1) > std::get<1>(r2) ||
  (std::get<1>(r1) == std::get<1>(r2) && std::get<0>(r1) < std::get<0>(r2));
}


