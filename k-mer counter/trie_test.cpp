//
//  trie_test.cpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 26/03/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//
#include "catch.hpp"
#include <stdio.h>
#include "trie.hpp"

TEST_CASE("trie works as expected", "[trie]") {
  KmerTrie trie;
  auto result = trie.GetAllKmers(5);
  
  REQUIRE(result.size() == 0);
 
  trie.InsertKmer("CCAAA");
  trie.InsertKmer("AAAAA");
  trie.InsertKmer("AAAAC");
  trie.InsertKmer("AAAAA");
  trie.InsertKmer("AAAAA");
  trie.InsertKmer("ACGTN");
  trie.InsertKmer("CCAAA");
  trie.InsertKmer("CCAAA");
  trie.InsertKmer("CCAAA");
  trie.InsertKmer("ACGTN");
  
  result = trie.GetAllKmers(5);
  REQUIRE(result.size() == 4);
  
  result = trie.GetTopKmers(2, 5);
  
  REQUIRE(result.size() == 2);
  KmerResult res1 = *(result.begin());
  KmerResult res2 = *(++(result.begin()));
  REQUIRE(res1.first == "CCAAA");
  REQUIRE(res2.first == "AAAAA");
  REQUIRE(res1.second == 4);
  REQUIRE(res2.second == 3);
  
  KmerTrie trie2;
  std::string buffer = "CCAAAACCAAAAAAAAAAAAACACACACACGTNCCAAAA";
  for (int i = 0; i < buffer.size() - 5; ++i) {
    trie2.InsertKmer(buffer.c_str()+i, 6, 0);
  }
  
  result = trie2.GetAllKmers(6);
  const std::string exp_kmers[22] = {"AAAAAA", "ACACAC", "CCAAAA", "CACACA",
    "AAAAAC", "AAAACA", "AAAACC", "AAACAC", "AAACCA", "AACACA", "AACCAA",
    "ACACGT", "ACCAAA", "ACGTNC", "CAAAAA", "CAAAAC", "CACACG", "CACGTN",
    "CGTNCC", "GTNCCA", "NCCAAA", "TNCCAA"};
  const int exp_count[22] = {8,3,3,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

  
  REQUIRE(result.size() == 22);
  
  int i = 0;
  for (auto &cur_res : result) {
    REQUIRE(cur_res.first == exp_kmers[i]);
    REQUIRE(cur_res.second == exp_count[i]);
    ++i;
  }
  
  result = trie2.GetTopKmers(5, 6);

  REQUIRE(result.size() == 5);
  
  i = 0;
  for (auto &cur_res : result) {
    REQUIRE(cur_res.first == exp_kmers[i]);
    REQUIRE(cur_res.second == exp_count[i]);
    ++i;
  }

}