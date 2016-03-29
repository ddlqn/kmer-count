//
//  main.cpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 23/03/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "trie.hpp"
#include "buffer.hpp"
#include <boost/circular_buffer.hpp>
#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>

int FillKmerTrie(std::ifstream &in_file, long k, KmerTrie &trie);
int FillKmerTrie_boost(std::ifstream &in_file, long k, KmerTrie &trie);
int FillKmerTrie_parallel(std::ifstream &in_file, long k, KmerTrie &trie);
int FillKmerTrieFromBuffer(char * buffer, long k, long start, long end, KmerTrie &trie);

int main(int argc, const char * argv[]) {
  if (argc != 4) {
    std::cerr << "usage: count <input_file> <num_kmers> <kmer_length>" << std::endl;
    return 0;
  }
  
  std::ifstream inFile(argv[1]);
  if (inFile.fail()) {
    std::cerr << "could not open input file " << argv[1] << std::endl;
    return -1;
  }
  
  long topN = strtol(argv[2], NULL, 10);
  long k = strtol(argv[3], NULL, 10);
  
  if (topN <= 0 || k <= 0) {
    std::cerr << "value(s) for num_kmers and/or kmer_length are invalid" << std::endl;
  }
  
  KmerTrie trie;
  auto start = std::chrono::steady_clock::now();

  if (FillKmerTrie(inFile, k, trie)) {
    inFile.close();
    return -1;
  }
  inFile.close();
  
  auto end = std::chrono::steady_clock::now();
  auto diff = end - start;
  
  std::cout << "using custom class: " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
  std::cout << "The trie contains the following 5-mers:" << std::endl;
  auto foo = trie.GetTopKmers(topN,k);
  for (auto &it : foo) {
    std::cout << std::get<0>(it) << "," << std::get<1>(it) << std::endl;
  }

  inFile.open(argv[1]);
  KmerTrie trie2;
  start = std::chrono::steady_clock::now();
  
  if (FillKmerTrie_boost(inFile, k, trie2)) {
    inFile.close();
    return -1;
  }

  inFile.close();
  
   end = std::chrono::steady_clock::now();
   diff = end - start;
  
  std::cout << "using boost class: " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
  std::cout << "The trie contains the following 5-mers:" << std::endl;
  foo = trie2.GetTopKmers(topN,k);
//  auto foo = trie.getAllKmers(k);
  for (auto &it : foo) {
    std::cout << std::get<0>(it) << "," << std::get<1>(it) << std::endl;
  }
  
  inFile.open(argv[1]);
  KmerTrie trie3;

  start = std::chrono::steady_clock::now();
  
  FillKmerTrie_parallel(inFile, k, trie3);
  inFile.close();

  end = std::chrono::steady_clock::now();
  diff = end - start;

  std::cout << "using threads: " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
  std::cout << "The trie contains the following 5-mers:" << std::endl;
  foo = trie2.GetTopKmers(topN,k);
  //  auto foo = trie.getAllKmers(k);
  for (auto &it : foo) {
    std::cout << std::get<0>(it) << "," << std::get<1>(it) << std::endl;
  }

  return 0;
}

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
      std::cout << "." << std::flush;
    }
  }
  std::cout << std::endl;
  return 0;
}

int FillKmerTrie_boost(std::ifstream &in_file, long k, KmerTrie &trie) {
  boost::circular_buffer<char> buffer((unsigned int)k);
  
  int i = 0;
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
      std::cout << "." << std::flush;
    }
  }
  std::cout << std::endl;
  return 0;
}

int FillKmerTrie_parallel(std::ifstream &in_file, long k, KmerTrie &trie) {
  const long chunk_size = 10000000;
  int n_threads = std::thread::hardware_concurrency();
  int buffer_size = n_threads*chunk_size;
  
  char * buffer = new char[buffer_size];
  
  in_file.read(buffer, buffer_size);
  if (in_file.gcount() < k) {
    std::cout << "input shorter than k-mer length (" << k << ")." << std::endl;
    return -1;
  }
  int i = buffer_size;
  while (in_file.gcount() > 0) {
    std::vector<std::thread> threads;

    for (int i = 0; i < n_threads && in_file.gcount() > i*chunk_size; ++i) {
      long end = (in_file.gcount() <= (i+1)*chunk_size ? in_file.gcount()-k+1 : (i+1)*chunk_size);
      threads.push_back(std::thread(FillKmerTrieFromBuffer, buffer, k, i*chunk_size, end, std::ref(trie)));
    }
    for (auto& thread : threads) {
      thread.join();
    }
    if (in_file.gcount() == buffer_size) {
      in_file.seekg(-(k-1), std::ios_base::cur);
    }
    in_file.read(buffer, buffer_size);
    i += buffer_size;
    if (!(i%10000000)) {
      std::cout << "." << std::flush;
    }
  }
  std::cout << std::endl;
  return 0;
}

int FillKmerTrieFromBuffer(char * buffer, long k, long start, long end, KmerTrie &trie) {
//  char * buf = "abcdefghijklmnopqrstuvwxyz";
  for (long i = start; i < end; ++i) {
//    std::cout << std::string(buf+i, k) << std::endl;
    trie.InsertKmer(buffer+i, k);
  }
  return 0;
}