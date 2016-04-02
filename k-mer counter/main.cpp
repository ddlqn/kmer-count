//
//  main.cpp
//  k-mer counter
//
//  main function of k-mer counter. Parses commandline arguments and then
//  creates a KmerCounter object that does the actual work. Finally, prints
//  the results.
//
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "trie.hpp"
#include "counter.hpp"
#include "util.hpp"


int main(int argc, const char * argv[]) {
  if (argc != 4) {
    std::cerr << "usage: count <input_file> <num_kmers> <kmer_length>"
              << std::endl;
    return 0;
  }
  
  unsigned int top_N = (unsigned int) strtol(argv[2], NULL, 10);
  int k = (int) strtol(argv[3], NULL, 10);
  
  if (top_N <= 0 || k <= 0) {
    std::cerr << "value(s) for num_kmers and/or kmer_length are invalid"
              << std::endl;
    return -1;
  } else if (k > 27) {
    std::cerr << "this program only works reliably up to k-mer length 27"
              << std::endl;
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

  return 0;
}
