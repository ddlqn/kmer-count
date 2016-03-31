//
//  tools.cpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 29/03/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
#include "util.hpp"

int FillKmerTrie(std::ifstream &in_file, long k, KmerTrie &trie) {
  CircularBuffer buffer((unsigned int)k);
  
  int i = 0;
  char base;
  while (i < k-1 && (in_file >> base)) {
    buffer.push_back(base);
    ++i;
  }
  if (!in_file || in_file.peek() == EOF) {
    std::cout << "input shorter than k-mer length (" << k << ")." << std::endl;
    return -1;
  }
  while (in_file >> base) {
    buffer.push_back(base);
    trie.InsertKmer(buffer.begin(), buffer.end());
    ++i;
    if (!(i%10000000)) {
      std::cerr << "." << std::flush;
    }
  }
  std::cerr << std::endl;
  return 0;
}

int FillKmerTrie_boost(std::ifstream &in_file, long k, KmerTrie &trie) {
  boost::circular_buffer<char> buffer((unsigned int)k);
  
  unsigned long long i = 0;
  char base;
  while (i < k-1 && (in_file >> base)) {
    buffer.push_back(base);
    i++;
  }
  if (!in_file || in_file.peek() == EOF) {
    std::cout << "input shorter than k-mer length (" << k << ")." << std::endl;
    return -1;
  }
  while (in_file >> base) {
    buffer.push_back(base);
    trie.InsertKmer(buffer.begin(), buffer.end());
    ++i;
    if (!(i%10000000)) {
      std::cerr << "." << std::flush;
    }
  }
  std::cerr << std::endl;
  return 0;
}

int FillKmerTrie_parallel(std::ifstream &in_file, long k, KmerTrie &trie) {
  const long buffer_size = 1000000;
  
  char * buffer = new char[buffer_size];
  
  in_file.read(buffer, buffer_size);
  if (in_file.gcount() < k) {
    std::cout << "input shorter than k-mer length (" << k << ")." << std::endl;
    return -1;
  }
  unsigned long long i = buffer_size;
  while (in_file.gcount() > 0) {
    long end = in_file.gcount()-k+1;
    if (buffer[end+k-2] == '\n') {
      --end;
    }
    FillKmerTrieFromBuffer(buffer, k, 0, end, trie);
    if (in_file.gcount() == buffer_size) {
      in_file.seekg(-(k-1), std::ios_base::cur);
    }
    in_file.read(buffer, buffer_size);
    i += buffer_size;
    if (!(i%10000000)) {
      std::cerr << "." << std::flush;
    }
  }
  std::cerr << std::endl;
  return 0;
}

int FillKmerTrieFromBuffer(char * buffer, long k, long start, long end, KmerTrie &trie) {
  for (long i = start; i < end; ++i) {
    trie.InsertKmer(buffer+i, k);
  }
  return 0;
}

int FillHashTable(std::ifstream &in_file, std::map<unsigned long long, unsigned long long> &kmer_map, long k) {
  const long buffer_size = 1000000;
  char * buffer = new char[buffer_size];
  
  in_file.read(buffer, buffer_size);

  if (in_file.gcount() < k) {
    std::cout << "input shorter than k-mer length (" << k << ")." << std::endl;
    return -1;
  }
  unsigned long long i = buffer_size;
  while (in_file.gcount() > 0) {
    long end = in_file.gcount()-k+1;
    if (buffer[end+k-2] == '\n') {
      --end;
    }
    
    for (long i = 0; i < end; ++i) {
      unsigned long long hash = ComputeHash(buffer+i, k);
      try {
        kmer_map.at(hash) += 1;
      } catch (const std::out_of_range &oor) {
        kmer_map[hash] = 1;
      }
    }

    if (in_file.gcount() == buffer_size) {
      in_file.seekg(-(k-1), std::ios_base::cur);
    }
    in_file.read(buffer, buffer_size);
    i += buffer_size;
    if (!(i%10000000)) {
      std::cerr << "." << std::flush;
    }
  }
  std::cerr << std::endl;
  return 0;
}

unsigned long long ComputeHash(char * kmer, long k) {
  unsigned long long hash = 0;
  for (int i = 0; i < k; ++i) {
    hash *= 5;
    hash += BaseToIndex(kmer[i]);
  }
  return hash;
}

std::string ComputeKmerFromHash(unsigned long long hash, long k) {
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


KmerTrie::KmerResultSet GetTopKmersFromMap(std::map<unsigned long long, unsigned long long> &kmer_map, long k, unsigned long long n) {
  KmerTrie::KmerResultSet result_set;
  
  for (auto &it: kmer_map) {
    KmerTrie::KmerResult r(ComputeKmerFromHash(it.first, k), (unsigned long long)it.second);
    if (result_set.size() < n) {
      result_set.insert(r);
    } else if (r > *(result_set.rbegin())) {
      result_set.erase(*(result_set.rbegin()));
      result_set.insert(r);
    }
  }
  
  return result_set;
}

KmerTrie::KmerResultSet GetTopKmersFromFile(std::ifstream &in_file, long n, long k) {
  std::fstream tmp_files[25];
  for (int i = 0; i < 25; ++i) {
    tmp_files[i].open("tmp"+std::to_string(i), std::ofstream::out | std::ofstream::binary);
  }
  
  KmerTrie::KmerResultSet result_set;
  const long buffer_size = 1000000;
  char * buffer = new char[buffer_size];
  
  in_file.read(buffer, buffer_size);
  
  if (in_file.gcount() < k) {
    std::cerr << "input shorter than k-mer length (" << k << ")." << std::endl;
    return result_set;
  }
  unsigned long long i = buffer_size;
  while (in_file.gcount() > 0) {
    long end = in_file.gcount()-k+1;
    if (buffer[end+k-2] == '\n') {
      --end;
    }
    
    for (long i = 0; i < end; ++i) {
      unsigned long long hash = ComputeHash(buffer+i+2, k-2);
      tmp_files[ComputeHash(buffer+i,2)].write((char *) &hash, sizeof(unsigned long long));
    }
    
    if (in_file.gcount() == buffer_size) {
      in_file.seekg(-(k-1), std::ios_base::cur);
    }
    in_file.read(buffer, buffer_size);
    i += buffer_size;
    if (!(i%10000000)) {
      std::cerr << "." << std::flush;
    }
  }
  std::cerr << std::endl;
  
  for (int i = 0; i < 25; ++i) {
    tmp_files[i].close();
  }
  
  unsigned long long read = 0;
  for (int fi = 0; fi < 25; ++fi) {
    std::cerr << "reading file " << fi << " ";
    tmp_files[fi].open("tmp"+std::to_string(fi), std::fstream::in | std::fstream::binary);
    
//    std::map<long long, long long> kmer_map;
//    std::set<KmerPair, KmerPairCompare> kmer_set;
//    KmerSet kmer_set;
    std::vector<unsigned long long> kmers;
    unsigned long long hash;
    while (tmp_files[fi].read((char *)&hash, sizeof(unsigned long long))) {
//      KmerPair cur_kmer(hash,1);
      kmers.push_back(hash);
      
//      auto el = kmer_set.insert(cur_kmer);
//      KmerSet::iterator hint = el.first;
//      ++hint;
//      if (!el.second) {
//        cur_kmer.second += (*(el.first)).second;
//        kmer_set.erase(el.first);
//        kmer_set.insert(hint, cur_kmer);
//      }
      
//      try {
//        kmer_map.at(hash) += 1;
//      } catch (const std::out_of_range &oor) {
//        kmer_map[hash] = 1;
//      }
      ++read;
      if (!(read%10000000)) {
        std::cerr << "*" << std::flush;
      }
    }
    std::cerr << '\r'; //std::endl;
    std::sort(kmers.begin(), kmers.end());

    for (auto lb = kmers.begin(), ub = kmers.begin(); lb != kmers.end(); lb = ub) {
      ub = std::upper_bound(lb, kmers.end(), *lb);
      KmerTrie::KmerResult r(ComputeKmerFromHash(fi,2)+ComputeKmerFromHash(*lb, k-2), ub-lb);

      if (result_set.size() < n) {
        result_set.insert(r);
      } else if (r > *(result_set.rbegin())) {
        result_set.erase(*(result_set.rbegin()));
        result_set.insert(r);
      }
    }

////    for (auto &it: kmer_map) {
//    for (auto &it: kmer_set) {
//      KmerTrie::KmerResult r(ComputeKmerFromHash(fi,2)+ComputeKmerFromHash(it.first, k-2), (unsigned int)it.second);
//      if (result_set.size() < n) {
//        result_set.insert(r);
//      } else if (r > *(result_set.rbegin())) {
//        result_set.erase(*(result_set.rbegin()));
//        result_set.insert(r);
//      }
//    }
    
    tmp_files[fi].close();
    std::remove(("tmp"+std::to_string(fi)).c_str());
  }

  return result_set;
}