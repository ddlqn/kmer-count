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
  KmerTrie::KmerResult res1 = *(result.begin());
  KmerTrie::KmerResult res2 = *(++(result.begin()));
  REQUIRE(std::get<0>(res1) == "CCAAA");
  REQUIRE(std::get<0>(res2) == "AAAAA");
  REQUIRE(std::get<1>(res1) == 4);
  REQUIRE(std::get<1>(res2) == 3);

}