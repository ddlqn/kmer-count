//
//  KmerCounter.cpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 31/03/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <vector>
#include "counter.hpp"
#include "util.hpp"


KmerCounter::KmerCounter(std::string file_name) {
  in_file.open(file_name);
  if (!in_file.good()) {
    throw "could not open input file "+file_name;
  }

  buffer = new char[buffer_size];
}

KmerCounter::~KmerCounter() {
  if (in_file.is_open()) {
    in_file.close();
  }
  delete [] buffer;
}

KmerResultSet KmerCounter::GetTopKmers(unsigned int top_N, int k) {
  if (!in_file.good()) {
    reset();
  }
  
  if (k <= 10) {
    ComputeTopKmersUsingTrie(top_N, k);
  } else {
    ComputeTopKmersUsingFiles(top_N, k);
  }
  return result_set;
}

void KmerCounter::ComputeTopKmersUsingTrie(unsigned int top_N,
                                           int k) {
  KmerTrie trie;
  FillKmerTrie(k, trie);
  result_set = trie.GetTopKmers(top_N,k);
}

void KmerCounter::FillKmerTrie(int k, KmerTrie &trie) {
  unsigned long long file_length = GetFileLength(in_file);
  if (file_length < k) {
    throw "input shorter than k-mer length ("+std::to_string(k)+").";
  }
  
  std::cerr << "reading input file";
  file_length -= k-1;
  if (file_length < 10) {
    file_length = 10;
  }
  in_file.read(buffer, buffer_size);
  unsigned long long read = 0;
  while (in_file.gcount() > 0) {
    long end = in_file.gcount()-k+1;
    if (buffer[end+k-2] == '\n') {
      --end;
    }

    for (long i = 0; i < end; ++i) {
      trie.InsertKmer(buffer+i, k);

      ++read;
      if (!(read%(file_length/10))) {
        std::cerr << "." << std::flush;
      }
    }
    
    FillBuffer(-(k-1));
  }
  std::cerr << "\33[2K\r";
}

inline void KmerCounter::FillBuffer(long offset) {
  if (in_file.gcount() == buffer_size) {
    in_file.seekg(offset, in_file.cur);
  }
  in_file.read(buffer, buffer_size);
}

void KmerCounter::ComputeTopKmersUsingFiles(unsigned int top_N, int k) {
  
  WriteKmersToTempFiles(k);
  
  for (int file_index = 0; file_index < number_of_files; ++file_index) {
    ReadTempFile(file_index, top_N, k);
    std::remove(("tmp"+std::to_string(file_index)).c_str());
  }
  
}

void KmerCounter::WriteKmersToTempFiles(int k) {
  
  unsigned long long file_length = GetFileLength(in_file);
  if (file_length < k) {
    throw "input shorter than k-mer length ("+std::to_string(k)+").";
  }
  file_length -= k-1;
  
  std::ofstream tmp_files[number_of_files];
  for (int i = 0; i < number_of_files; ++i) {
    tmp_files[i].open("tmp"+std::to_string(i),
                      std::ofstream::out | std::ofstream::binary);
  }
  
  std::cerr << "reading input file";

  in_file.read(buffer, buffer_size);
  
  unsigned long long read = 0;
  while (in_file.gcount() > 0) {
    long end = in_file.gcount()-k+1;
    if (buffer[end+k-2] == '\n') {
      --end;
    }
    
    for (long i = 0; i < end; ++i) {
      unsigned long long hash = ComputeHash(buffer+i+2, k-2);
      tmp_files[ComputeHash(buffer+i,2)].write((char *) &hash,
                                               sizeof(unsigned long long));
      ++read;
      if (!(read%(file_length/10))) {
        std::cerr << "." << std::flush;
      }
    }
    
    FillBuffer(-(k-1));
  }
  std::cerr << "\33[2K\r";
  
  for (int i = 0; i < number_of_files; ++i) {
    tmp_files[i].close();
  }

}

void KmerCounter::ReadTempFile(int file_index, unsigned int top_N, int k) {
  std::ifstream tmp_file;

  std::cerr << "reading file " << file_index << " ";
  tmp_file.open("tmp"+std::to_string(file_index),
                std::fstream::in | std::fstream::binary);
  
  // We want to give some indication of progess every 10%
  unsigned long long status_update =
    GetFileLength(tmp_file)/sizeof(unsigned long long);
  status_update = (status_update < 10 ? 1 : status_update/10);
  unsigned long long read = 0;
  
  std::vector<unsigned long long> kmers;
  unsigned long long hash;
  while (tmp_file.read((char *)&hash, sizeof(unsigned long long))) {
    kmers.push_back(hash);
    
    ++read;
    if (!(read%status_update)) {
      std::cerr << "*" << std::flush;
    }
  }
  std::sort(kmers.begin(), kmers.end());
  
  for (
       auto lower_bound = kmers.begin(), upper_bound = kmers.begin();
       lower_bound != kmers.end();
       lower_bound = upper_bound) {
    upper_bound = std::upper_bound(lower_bound, kmers.end(), *lower_bound);
    KmerResult kmer_result(ComputeKmerFromHash(file_index,2)
                           +ComputeKmerFromHash(*lower_bound, k-2),
                           upper_bound-lower_bound);
    
    if (result_set.size() < top_N) {
      result_set.insert(kmer_result);
    } else if (kmer_result > *(result_set.rbegin())) {
      result_set.erase(*(result_set.rbegin()));
      result_set.insert(kmer_result);
    }
  }
  
  std::cerr << "\33[2K\r";
  tmp_file.close();
}

void KmerCounter::reset() {
  in_file.clear();
  in_file.seekg(0, in_file.beg);
  result_set.clear();
}