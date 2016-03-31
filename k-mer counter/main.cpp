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
#include "counter.hpp"
#include "util.hpp"


int main(int argc, const char * argv[]) {
  if (argc != 4) {
    std::cerr << "usage: count <input_file> <num_kmers> <kmer_length>" << std::endl;
    return 0;
  }
  
  long top_N = strtol(argv[2], NULL, 10);
  long k = strtol(argv[3], NULL, 10);
  
  if (top_N <= 0 || k <= 0) {
    std::cerr << "value(s) for num_kmers and/or kmer_length are invalid" << std::endl;
  }

  try {
    KmerCounter counter(argv[1]);
    
    KmerResultSet result_set = counter.GetTopKmers(top_N, k);
    
    for (auto &it : result_set) {
      std::cout << it.first << "," << it.second << std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    return -1;
  }
  
  
  
  
//  if (k < 13) {
////  if (false) {
//      KmerTrie trie;
//      auto start = std::chrono::steady_clock::now();
//    
//      if (FillKmerTrie_parallel(in_file, k, trie)) {
//        in_file.close();
//        return -1;
//      }
//      in_file.close();
//    
//      auto end = std::chrono::steady_clock::now();
//      auto diff = end - start;
//    
//      std::cerr << "using custom class: " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
//      std::cerr << "The trie contains the following 5-mers:" << std::endl;
//      auto foo = trie.GetTopKmers(topN,k);
////      auto foo = trie.GetAllKmers(k);
//      for (auto &it : foo) {
//        std::cout << std::get<0>(it) << "," << std::get<1>(it) << std::endl;
//      }
//  } else {  
//    auto start = std::chrono::steady_clock::now();
//    //  KmerTrie::KmerResultSet result_set = GetTopKmersFromFile(in_file, LONG_MAX, k);
//    KmerResultSet result_set = GetTopKmersFromFile(in_file, topN, k);
//    in_file.close();
//    
//    if (result_set.size() == 0 ) {
//      return -1;
//    }
//    
//    auto end = std::chrono::steady_clock::now();
//    auto diff = end - start;
//    
//    std::cerr << "using files: " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
//    std::cerr << "The trie contains the following 5-mers:" << std::endl;
//    for (auto &it : result_set) {
//      std::cout << std::get<0>(it) << "," << std::get<1>(it) << std::endl;
//    }
//  }
  
  return 0;
}
