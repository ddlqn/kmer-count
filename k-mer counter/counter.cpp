//
//  KmerCounter.cpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 31/03/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include <iostream>
#include <vector>
#include "counter.hpp"
#include "util.hpp"


KmerCounter::KmerCounter(std::string file_name) {
  in_file.open(file_name);
  if (!in_file.good()) {
    throw "could not open input file "+file_name;
  }
}

KmerCounter::~KmerCounter() {
  if (in_file.is_open()) {
    in_file.close();
  }
}

KmerResultSet KmerCounter::GetTopKmers(unsigned long top_N, unsigned long k) {
  if (k < 13) {
    ComputeTopKmersUsingTrie(top_N, k);
  } else {
    ComputeTopKmersUsingFiles(top_N, k);
  }
  return result_set;
}

void KmerCounter::ComputeTopKmersUsingTrie(unsigned long top_N,
                                           unsigned long k) {
  KmerTrie trie;
  FillKmerTrie(k, trie);
  result_set = trie.GetTopKmers(top_N,k);
}

void KmerCounter::FillKmerTrie(unsigned long k, KmerTrie &trie) {
  const long buffer_size = 1000000;
  
  char * buffer = new char[buffer_size];
  
  in_file.read(buffer, buffer_size);
  if (in_file.gcount() < k) {
    throw "input shorter than k-mer length ("+std::to_string(k)+").";
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
}

int KmerCounter::FillKmerTrieFromBuffer(char * buffer, long k, long start,
                                        long end, KmerTrie &trie) {
  for (long i = start; i < end; ++i) {
    trie.InsertKmer(buffer+i, k);
  }
  return 0;
}

void KmerCounter::ComputeTopKmersUsingFiles(unsigned long n, unsigned long k) {
  std::fstream tmp_files[25];
  for (int i = 0; i < 25; ++i) {
    tmp_files[i].open("tmp"+std::to_string(i),
                      std::ofstream::out | std::ofstream::binary);
  }
  
  const long buffer_size = 1000000;
  char * buffer = new char[buffer_size];
  
  in_file.read(buffer, buffer_size);
  
  if (in_file.gcount() < k) {
    throw "input shorter than k-mer length ("+std::to_string(k)+").";
  }
  unsigned long long i = buffer_size;
  while (in_file.gcount() > 0) {
    long end = in_file.gcount()-k+1;
    if (buffer[end+k-2] == '\n') {
      --end;
    }
    
    for (long i = 0; i < end; ++i) {
      unsigned long long hash = ComputeHash(buffer+i+2, k-2);
      tmp_files[ComputeHash(buffer+i,2)].write((char *) &hash,
                                               sizeof(unsigned long long));
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
    tmp_files[fi].open("tmp"+std::to_string(fi),
                       std::fstream::in | std::fstream::binary);
    
    std::vector<unsigned long long> kmers;
    unsigned long long hash;
    while (tmp_files[fi].read((char *)&hash, sizeof(unsigned long long))) {
      kmers.push_back(hash);
      
      ++read;
      if (!(read%10000000)) {
        std::cerr << "*" << std::flush;
      }
    }
    std::cerr << '\r';
    std::sort(kmers.begin(), kmers.end());
    
    for (auto lower_bound = kmers.begin(), upper_bound = kmers.begin();
         lower_bound != kmers.end(); lower_bound = upper_bound) {
      upper_bound = std::upper_bound(lower_bound, kmers.end(), *lower_bound);
      KmerResult kmer_result(ComputeKmerFromHash(fi,2)
                             +ComputeKmerFromHash(*lower_bound, k-2),
                             upper_bound-lower_bound);
      
      if (result_set.size() < n) {
        result_set.insert(kmer_result);
      } else if (kmer_result > *(result_set.rbegin())) {
        result_set.erase(*(result_set.rbegin()));
        result_set.insert(kmer_result);
      }
    }
    
    tmp_files[fi].close();
    std::remove(("tmp"+std::to_string(fi)).c_str());
  }
  
}

void KmerCounter::reset() {
  in_file.seekg(0, std::ios_base::beg);
  in_file.clear();
  result_set.clear();
}