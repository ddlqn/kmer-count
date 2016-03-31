//
//  main.cpp
//  k-mer counter
//
//  main function of k-mer counter.
//
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include "trie.hpp"
#include "buffer.hpp"
#include "util.hpp"


int main(int argc, const char * argv[]) {
  if (argc != 4) {
    std::cerr << "usage: count <input_file> <num_kmers> <kmer_length>" << std::endl;
    return 0;
  }
  
  std::ifstream in_file(argv[1]);
  if (in_file.fail()) {
    std::cerr << "could not open input file " << argv[1] << std::endl;
    return -1;
  }
  
  long topN = strtol(argv[2], NULL, 10);
  long k = strtol(argv[3], NULL, 10);
  
  if (topN <= 0 || k <= 0) {
    std::cerr << "value(s) for num_kmers and/or kmer_length are invalid" << std::endl;
  }
  
  if (k < 13) {
//  if (false) {
      KmerTrie trie;
      auto start = std::chrono::steady_clock::now();
    
      if (FillKmerTrie_parallel(in_file, k, trie)) {
        in_file.close();
        return -1;
      }
      in_file.close();
    
      auto end = std::chrono::steady_clock::now();
      auto diff = end - start;
    
      std::cerr << "using custom class: " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
      std::cerr << "The trie contains the following 5-mers:" << std::endl;
//      auto foo = trie.GetTopKmers(topN,k);
      auto foo = trie.GetAllKmers(k);
      for (auto &it : foo) {
        std::cout << std::get<0>(it) << "," << std::get<1>(it) << std::endl;
      }
  } else {
//
//  in_file.open(argv[1]);
//  std::map<long long, long long> kmer_map;
//  auto start = std::chrono::steady_clock::now();
//  if(FillHashTable(in_file, kmer_map, k)) {
//    in_file.close();
//    return -1;
//  }
//  
//  in_file.close();
//  
//  auto end = std::chrono::steady_clock::now();
//  auto diff = end - start;
//
//  std::cerr << "using hash map: " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
//  std::cerr << "The trie contains the following 5-mers:" << std::endl;
//    auto foo = GetTopKmersFromMap(kmer_map, k, topN);
////  foo = GetTopKmersFromMap(kmer_map, k, LLONG_MAX);
//  for (auto &it : foo) {
//    std::cout << std::get<0>(it) << "," << std::get<1>(it) << std::endl;
//  }
//  
//  in_file.open(argv[1]);
  
    auto start = std::chrono::steady_clock::now();
    //  KmerTrie::KmerResultSet result_set = GetTopKmersFromFile(in_file, LONG_MAX, k);
    KmerTrie::KmerResultSet result_set = GetTopKmersFromFile(in_file, topN, k);
    in_file.close();
    
    if (result_set.size() == 0 ) {
      return -1;
    }
    
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    
    std::cerr << "using files: " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
    std::cerr << "The trie contains the following 5-mers:" << std::endl;
    for (auto &it : result_set) {
      std::cout << std::get<0>(it) << "," << std::get<1>(it) << std::endl;
    }
  }
  return 0;
}
