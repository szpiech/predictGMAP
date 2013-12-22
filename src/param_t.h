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
#ifndef __PARAM_T_H__
#define __PARAM_T_H__

#include <string>
#include <iostream>
#include <map>
#include <cctype>

using namespace std;

const string ARG_HELP = "--help";

class param_t
{
 public:

  bool addFlag(string flag, bool value, string label, string description);
  bool addFlag(string flag, double value, string label, string description);
  bool addFlag(string flag, int value, string label, string description);
  bool addFlag(string flag, char value, string label, string description);
  bool addFlag(string flag, string value, string label, string description);
  bool addFlag(string flag, const char value[], string label, string description);
  
  void printHelp();

  bool parseCommandLine(int argc, char *argv[]);
  
  bool getBoolFlag(string flag);
  double getDoubleFlag(string flag);
  int getIntFlag(string flag);
  char getCharFlag(string flag);
  string getStringFlag(string flag);

  param_t();


 private:
  
  map<string,bool> argb;
  map<string,double> argd;
  map<string,int> argi;
  map<string,char> argch;
  map<string,string> args;

  map<string,string> help;
  map<string,bool> isSet;

  bool goodDouble(string str);
  bool goodInt(string str);
  bool goodChar(string str);
};

#endif
