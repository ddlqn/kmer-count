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
  CircularBuffer buffer((unsigned int)k);
  
  int i = 0;
  char base;
  while (i < k-1 && (inFile >> base)) {
    buffer.push_back(base);
    ++i;
  }
  if (!inFile || inFile.peek() == EOF) {
    std::cout << "input shorter than k-mer length (" << k << ")." << std::endl;
    inFile.close();
    return 0;
  }
  while (inFile >> base) {
    buffer.push_back(base);
    trie.InsertKmer(buffer.begin(), buffer.end());
    ++i;
    if (!(i%10000000)) {
      std::cout << "." << std::flush;
    }
  }
  std::cout << std::endl;
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
  boost::circular_buffer<char> buffer2((unsigned int)k);
  
  i = 0;
  while (i < k-1 && (inFile >> base)) {
    buffer2.push_back(base);
    i++;
  }
  if (!inFile || inFile.peek() == EOF) {
    std::cout << "input shorter than k-mer length (" << k << ")." << std::endl;
    inFile.close();
    return 0;
  }
  while (inFile >> base) {
    buffer2.push_back(base);
    trie2.InsertKmer(buffer2.begin(), buffer2.end());
    ++i;
    if (!(i%10000000)) {
      std::cout << "." << std::flush;
    }
  }
  std::cout << std::endl;
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

  return 0;
}
