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
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include "param_t.h"
#include "mapdata_t.h"

using namespace std;

const string ARG_REFMAP = "--ref";
const string DEFAULT_REFMAP = "__reference_map";
const string HELP_REFMAP = "The reference genetic map file with format: <chr#> <locusID> <genetic pos> <physical pos>";

const string ARG_SNPLIST = "--query";
const string DEFAULT_SNPLIST = "__snp_list";
const string HELP_SNPLIST = "A list of physical positions at which you desire a genetic map.";

const string ARG_OUTFILE = "--out";
const string DEFAULT_OUTFILE = "predictGMAP.outfile.map";
const string HELP_OUTFILE = "The name of the output file. Default: predictGMAP.outfile.map";

const string ARG_MAXGAP = "--max-gap";
const double DEFAULT_MAXGAP = 18848.64;
const string HELP_MAXGAP = "If a gap of > MAX_GAP is encountered in the reference map file, do not interpolate in between. Default: 18848.64, dervied from the standard deviation in gap lengths from the HapMap genetic map.";

int main(int argc, char *argv[])
{
    param_t params;

    params.addFlag(ARG_REFMAP, DEFAULT_REFMAP, "refmapLabel", HELP_REFMAP);
    params.addFlag(ARG_SNPLIST, DEFAULT_SNPLIST, "snplistLabel", HELP_SNPLIST);
    params.addFlag(ARG_OUTFILE, DEFAULT_OUTFILE, "outfileLabel", HELP_OUTFILE);
    params.addFlag(ARG_MAXGAP, DEFAULT_MAXGAP, "outfileLabel", HELP_MAXGAP);

    try
    {
        params.parseCommandLine(argc, argv);
    }
    catch (...)
    {
        return 1;
    }

    string refmapfile = params.getStringFlag(ARG_REFMAP);
    string snplistfile = params.getStringFlag(ARG_SNPLIST);
    string outfile = params.getStringFlag(ARG_OUTFILE);
    double MAXGAP = params.getDoubleFlag(ARG_MAXGAP);



    // Start to read in query data
    ifstream fin;
    fin.open(snplistfile.c_str());
    if (fin.fail())
    {
        cerr << "ERROR: could not open " << snplistfile << " for reading.\n";
        return 1;
    }

    //Count the number of positions
    int fileStart = fin.tellg();
    int dataSize = 0;
    double num;
    while (1)
    {
        fin >> num;
        if (fin.eof()) break;
        dataSize++;
    }

    cerr << "Reading " << dataSize << " query positions.\n";

    fin.clear();
    fin.seekg(fileStart);

    int *query = new int[dataSize];

    for (int i = 0; i < dataSize; i++)
    {
        fin >> query[i];
    }

    fin.close();

    ofstream fout;
    fout.open(outfile.c_str());
    if (fout.fail())
    {
        cerr << "ERROR: could not open " << outfile << " for reading.\n";
    }

    MapData md(refmapfile, MAXGAP);
    double gPos;
    string locName, chr;
    int current_index = 0;

    for (int i = 0; i < dataSize; i++)
    {
        if (md.getMapInfo(query[i], gPos, locName, chr, current_index))
        {
            fout << chr << "\t"
                 << locName << "\t"
                 << setprecision(15) << gPos << "\t"
                 << query[i] << endl;
        }
    }
    fout.close();

    return 0;
}
