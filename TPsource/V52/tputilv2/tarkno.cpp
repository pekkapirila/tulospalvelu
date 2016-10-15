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

int tarkno(int kno)
{
   int t,k;

   t = 3 * (kno / 1000);
   k = kno - 1000 * (kno / 1000);
   t = t + k / 100;
   k = k - 100 * (k / 100);
   t = t + 7 * (k / 10) + 3 * (k % 10);
   return((130 - t) % 10);
}


