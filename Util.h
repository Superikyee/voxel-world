#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <glm/vec2.hpp>

#include <cmath>
#include <random>
#include <iostream>
#include <vector>


unsigned int index3d(unsigned int X, unsigned int Y, unsigned int Z, unsigned int Height, unsigned int Depth) {
  return X * Height * Depth + Y * Depth + Z;
}


unsigned int index2d(unsigned int X, unsigned int Y, unsigned int Width) {
  return Y * Width + X;
}


template<typename T>
///\brief Checks if any of the vectors components (or even the vector sizes) are different from other.
bool areVectorsDifferent(std::vector<T> vec1, std::vector<T> vec2)
{
  if (vec1.size() != vec2.size()) {
    return true;
  } else {
    for (unsigned int i = 0; i < vec1.size(); i++) {
      if (vec1[i] != vec2[i]) {
        return true;
      }
    }
  }
  return false;
}

#endif // UTIL_H_INCLUDED
