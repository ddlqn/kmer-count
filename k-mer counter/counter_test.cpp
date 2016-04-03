//
//  counter_test.cpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 01/04/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include <cstdio>
#include "catch.hpp"
#include "counter.hpp"

void LoadExpectedResultSet(std::string file_name,
                                    KmerResultSet &result_set) {
  std::ifstream in_file(file_name);
  std::string kmer;
  int count;
  while (in_file >> kmer >> count) {
    result_set.insert(KmerResult(kmer, count));
  }
  in_file.close();
}

TEST_CASE("test KmerCounter class", "[KmerCounter]") {
  REQUIRE_THROWS(KmerCounter c("inexistent_file.txt"));
  
  KmerCounter counter("test_data/test.txt");
  
  KmerResultSet result_set = counter.GetTopKmers(5, 6);
  
  REQUIRE(result_set.size() == 5);

  const std::string exp_kmers[22] = {"AAAAAA", "ACACAC", "CCAAAA", "CACACA",
    "AAAAAC", "AAAACA", "AAAACC", "AAACAC", "AAACCA", "AACACA", "AACCAA",
    "ACACGT", "ACCAAA", "ACGTNC", "CAAAAA", "CAAAAC", "CACACG", "CACGTN",
    "CGTNCC", "GTNCCA", "NCCAAA", "TNCCAA"};
  const int exp_count[22] = {8,3,3,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

  int i = 0;
  for (auto &cur_res : result_set) {
    REQUIRE(cur_res.first == exp_kmers[i]);
    REQUIRE(cur_res.second == exp_count[i]);
    ++i;
  }

  result_set = counter.GetTopKmers(100, 6);
  
  REQUIRE(result_set.size() == 22);
  i = 0;
  for (auto &cur_res : result_set) {
    REQUIRE(cur_res.first == exp_kmers[i]);
    REQUIRE(cur_res.second == exp_count[i]);
    ++i;
  }
  
  KmerCounter counter2("test_data/long.txt");
  const std::string exp_kmers2[7] = {"AAAAAAAAAAAAAAAAAAAAAAAAAAA",
    "CCCCCCCCCCCCCCCCCCCCCCCCCCC", "GGGGGGGGGGGGGGGGGGGGGGGGGGG",
    "TTTTTTTTTTTTTTTTTTTTTTTTTTT", "ACACACACACACACACACACACACACA",
    "CACACACACACACACACACACACACAC", "NNNNNNNNNNNNNNNNNNNNNNNNNNN"};
  const int exp_count2[7] = {56,29,29,28,15,15,3};
  
  result_set = counter2.GetTopKmers(7, 27);
  
  REQUIRE(result_set.size() == 7);
  i = 0;
  for (auto &cur_res : result_set) {
    REQUIRE(cur_res.first == exp_kmers2[i]);
    REQUIRE(cur_res.second == exp_count2[i]);
    ++i;
  }
  
  KmerResultSet exp_kmers3;
  LoadExpectedResultSet("test_data/big_out_3.txt", exp_kmers3);
  KmerCounter counter3("test_data/big.txt");
  result_set = counter3.GetTopKmers(125, 3);
  
  REQUIRE(result_set.size() == exp_kmers3.size());
  
  for (auto it_res = result_set.begin(), it_exp = exp_kmers3.begin();
       it_res != result_set.end();
       ++it_res, ++it_exp) {
    REQUIRE((*it_res).first == (*it_exp).first);
    REQUIRE((*it_res).second == (*it_exp).second);
  }
  
  exp_kmers3.clear();
  LoadExpectedResultSet("test_data/big_out_12.txt", exp_kmers3);
  result_set = counter3.GetTopKmers(244140625, 12);
  
  REQUIRE(result_set.size() == exp_kmers3.size());

  for (auto it_res = result_set.begin(), it_exp = exp_kmers3.begin();
       it_res != result_set.end();
       ++it_res, ++it_exp) {
    REQUIRE((*it_res).first == (*it_exp).first);
    REQUIRE((*it_res).second == (*it_exp).second);
  }
  
}