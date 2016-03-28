//
//  buffer.hpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 26/03/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#ifndef buffer_hpp
#define buffer_hpp

#include <stdio.h>

class CircularBufferIterator;

class CircularBuffer {
  char * buffer;
  unsigned int capacity, size;
  int front, back;
  
  
public:
  CircularBuffer(unsigned int capacity);
  ~CircularBuffer();
  void push_back(char c);
  CircularBufferIterator begin();
  CircularBufferIterator end();
  const char & operator[](int index) const;
  
};

class CircularBufferIterator {
  const CircularBuffer & buffer;
  int offset;
public:
  CircularBufferIterator(const CircularBuffer & b, int o) : buffer(b), offset(o) {};
  ~CircularBufferIterator() {};
  CircularBufferIterator & operator++();
  CircularBufferIterator operator++(int i);
  bool operator==(const CircularBufferIterator & it);
  bool operator!=(const CircularBufferIterator & it);
  const char & operator*();
};

#endif /* buffer_hpp */
