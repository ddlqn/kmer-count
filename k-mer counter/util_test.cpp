//
//  util_test.cpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 01/04/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include <cstdio>
#include <string>
#include "catch.hpp"
#include "util.hpp"

TEST_CASE("test hash function", "[hash]") {
  std::string test_sequence = "AAAAANTGCA";
  REQUIRE(ComputeHash(test_sequence.c_str(), 5) == 0);
  REQUIRE(ComputeHash((test_sequence.c_str()+5), 5) == 2930);
  REQUIRE(ComputeHash(test_sequence.c_str(), 10) == 2930);
  
  REQUIRE(ComputeKmerFromHash(1234, 5) == "CNNCN");
  REQUIRE(ComputeKmerFromHash(987654321, 13) == "NACATCNNCNGNC");
  REQUIRE(ComputeKmerFromHash((unsigned long long)7450580596923828124, 27) == "NNNNNNNNNNNNNNNNNNNNNNNNNNN");
}

TEST_CASE("test base - index conversion", "[index]") {
  REQUIRE(BaseToIndex('a') == 0);
  REQUIRE(BaseToIndex('A') == BaseToIndex('a'));
  REQUIRE(BaseToIndex('c') == 1);
  REQUIRE(BaseToIndex('C') == BaseToIndex('c'));
  REQUIRE(BaseToIndex('g') == 2);
  REQUIRE(BaseToIndex('G') == BaseToIndex('g'));
  REQUIRE(BaseToIndex('t') == 3);
  REQUIRE(BaseToIndex('T') == BaseToIndex('t'));
  REQUIRE(BaseToIndex('n') == 4);
  REQUIRE(BaseToIndex('N') == BaseToIndex('n'));
  for (int i = 0; i < 256; i++) {
    if (i == 'a' || i == 'A' || i == 'c' || i == 'C' || i == 'g' || i == 'G' || i == 't' || i == 'T' || i == 'n' || i == 'N') {
      continue;
    }
    REQUIRE_THROWS(BaseToIndex(i));
  }
  
  REQUIRE(IndexToBase(0) == 'A');
  REQUIRE(IndexToBase(1) == 'C');
  REQUIRE(IndexToBase(2) == 'G');
  REQUIRE(IndexToBase(3) == 'T');
  REQUIRE(IndexToBase(4) == 'N');
  for (int i = 5; i < 257; i++) {
    REQUIRE_THROWS(IndexToBase(i));
  }
}

TEST_CASE("KmerResult comparisons", "[KmerResult]") {
  KmerResultCompare c;
  KmerResult r1("AAAAA", 20), r2("ACACA", 10), r3("TGCAN", 10);
  
  REQUIRE(c(r1,r2) == true);
  REQUIRE(c(r2,r1) == false);
  REQUIRE(c(r2,r3) == true);
  REQUIRE(c(r3,r2) == false);
  
  REQUIRE((r1 > r2) == true);
  REQUIRE((r2 > r1) == false);
  REQUIRE((r2 > r3) == true);
  REQUIRE((r3 > r2) == false);
}
