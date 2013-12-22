/*
  Copyright (C) 2013  Zachary A Szpiech (szpiech@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MAPDATA_T_H__
#define __MAPDATA_T_H__

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <cstdio>

using namespace std;

const int MISSING = -9999;

class MapData
{
 public:
  MapData(string filename, double mGap);
  ~MapData();
  bool getMapInfo(int queryPos, double& gPos, string& locName, string& c,int& current_locus);

 private:
  int* physicalPos;
  double* geneticPos;
  string* locusName;
  map<int,int> ppos2index;
  int nloci;
  string chr;

  double MAXGAP;

  int countFields(const string &str);
  void initMapData(int n);
  double interpolate(double x0, double y0, double x1, double y1, double query);
};

inline double MapData::interpolate(double x0, double y0, double x1, double y1, double query)
{
  return ( ( (y1-y0) / (x1-x0) ) * query + ( y0 - ((y1-y0) / (x1-x0)) * x0 ) ); 
}

#endif
