// Pekka Pirila's sports timekeeping program (Finnish: tulospalveluohjelma)
// Copyright (C) 2015 Pekka Pirila 

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// test program mttest.cpp, see mtreadme.txt for information
#include "mtrand.h"
#include <cstdio>

int main() {
  unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
  MTRand_int32 irand(init, length); // 32-bit int generator
// this is an example of initializing by an array
// you may use MTRand(seed) with any 32bit integer
// as a seed for a simpler initialization
  MTRand drand; // double in [0, 1) generator, already init

// generate the same numbers as in the original C test program
  std::printf("1000 32-bit integer random numbers:\n");
  for (int i = 0; i < 1000; ++i) {
    std::printf("%10lu ", irand());
    if ((i % 5) == 4) std::printf("\n");
  }
  std::printf("\n1000 random numbers in [0, 1):\n");
  for (int i = 0; i < 1000; ++i) {
    std::printf("%10.8f ", drand());
    if ((i % 5) == 4) std::printf("\n");
  }
}
