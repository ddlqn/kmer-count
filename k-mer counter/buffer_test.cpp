//
//  buffer_test.cpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 26/03/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <stdio.h>
#include "buffer.hpp"


TEST_CASE("buffer works as expected", "[buffer]") {
  CircularBuffer b(5);

//  REQUIRE(b.begin() == 0);
//  REQUIRE(b.end() == 5);
  REQUIRE_THROWS(b[0]);
  
  b.push_back('a');
  b.push_back('b');
  b.push_back('c');
  
  REQUIRE(*(b.begin()) == 'a');
//  REQUIRE(b.end() == 3);

  std::string str1;
  for (CircularBufferIterator it = b.begin(); !(it == b.end()); ++it) {
    str1.push_back(*it);
  }
  
  REQUIRE(str1 == "abc");
  
  b.push_back('d');
  b.push_back('e');
  
  REQUIRE(*(b.begin()) == 'a');
//  REQUIRE(b.end() == 5);
  
  b.push_back('f');
  REQUIRE(*(b.begin()) == 'b');
//  REQUIRE(b.end() == 6);
  
  std::string str2;
  for (CircularBufferIterator it = b.begin(); it != b.end(); ++it) {
    str2.push_back(*it);
  }
  
  REQUIRE(str2 == "bcdef");
}

