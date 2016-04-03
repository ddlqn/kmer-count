//
//  trie.cpp
//  k-mer counter
//
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include "trie.hpp"
#include <climits>
#include <iostream>


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


void KmerTrie::InsertKmer(const char * buf, int k, int pos) {
  if (pos == k) {
    count += 1;
  } else {
    int curIndex = BaseToIndex(*(buf+pos));
    if (!children[curIndex]) {
      children[curIndex] = new KmerTrie();
    }
    children[curIndex]->InsertKmer(buf, k, ++pos);
  }
}

KmerResultSet KmerTrie::GetTopKmers(unsigned int n, int k) {
  KmerResultSet result_set;
  std::string prefix;
  GetTopKmers(n, k, result_set, prefix);
  return result_set;
}

KmerResultSet KmerTrie::GetAllKmers(int k) {
  KmerResultSet result_set;
  std::string prefix;
  GetTopKmers(LLONG_MAX, k, result_set, prefix);
  return result_set;
}

void KmerTrie::GetTopKmers(long long int n,
                           int k,
                           KmerResultSet &result_set,
                           std::string &prefix) {
  if (prefix.length() == k) {
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

