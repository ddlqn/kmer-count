//
//  buffer.cpp
//  k-mer counter
//
//  Created by Daniel Dalquen on 26/03/2016.
//  Copyright © 2016 Daniel Dalquen. All rights reserved.
//

#include "buffer.hpp"

CircularBuffer::CircularBuffer(unsigned int capacity) {
  this->capacity = capacity;
  buffer = new char[capacity];
  size = front = back = 0;
}

CircularBuffer::~CircularBuffer() {
  delete [] buffer;
}

void CircularBuffer::push_back(char c) {
  buffer[back%capacity] = c;
  back = (back + 1) % capacity;
  if (size < capacity) {
    ++size;
  } else {
    front = (front + 1) % capacity;
  }
}

CircularBufferIterator CircularBuffer::begin() {
  return CircularBufferIterator(*this, front);
}

CircularBufferIterator CircularBuffer::end() {
  return CircularBufferIterator(*this, front < back ? back : capacity+back);
}

const char & CircularBuffer::operator[](int index) const {
  if (size == 0) {
    throw "The buffer is empty!";
  }
  return buffer[index%size];
}

/******************************
 * CircularBufferIterator implementation *
 ******************************/

CircularBufferIterator & CircularBufferIterator::operator++() {
  ++offset;
  return *this;
}

CircularBufferIterator CircularBufferIterator::operator++(int i) {
  CircularBufferIterator newit(*this);
  ++offset;
  return newit;
}

bool CircularBufferIterator::operator==(const CircularBufferIterator & it) {
  return &(this->buffer) == &(it.buffer) && this->offset == it.offset;
}

bool CircularBufferIterator::operator!=(const CircularBufferIterator & it) {
  return !(*this == it);
}

const char & CircularBufferIterator::operator*() {
  return buffer[offset];
}
