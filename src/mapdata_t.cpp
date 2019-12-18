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
#include "mapdata_t.h"

bool MapData::getMapInfo(int queryPos, double &gPos, string &locName, string &c, int &current_index)
{
    bool success = true;

    if (queryPos < physicalPos[0])
    {
        //cerr << "Skip low.\n";
        success = false;
    }
    else if (queryPos > physicalPos[nloci - 1])
    {
        //cerr << "Skip high.\n";
        //success = false;
        c = chr;
        gPos = this->interpolate(physicalPos[nloci - 2], geneticPos[nloci - 2],
                                 physicalPos[nloci - 1], geneticPos[nloci - 1],
                                 queryPos);
        char buffer[50];
        sprintf(buffer, "chr%s:%d", chr.c_str(), queryPos);
        locName = buffer;
    }
    else if (ppos2index.count(queryPos) > 0)
    {
        //cerr << "Exists:\t";
        int index = ppos2index[queryPos];
        gPos = geneticPos[index];
        locName = locusName[index];
        c = chr;
    }
    else
    {
        //cerr << "Calc:\t";
        int startIndex;
        int endIndex;
        for (current_index; current_index < nloci - 1; current_index++)
        {
            if (queryPos > physicalPos[current_index] && queryPos < physicalPos[current_index + 1])
            {
                startIndex = current_index;
                endIndex = current_index + 1;
                break;
            }
        }

        if (physicalPos[endIndex] - physicalPos[startIndex] > MAXGAP)
        {
            //cerr << physicalPos[ppos2index[endPos]] - physicalPos[ppos2index[startPos]] << " > " << MAXGAP << endl;
            return false;
        }

        c = chr;
        gPos = this->interpolate(physicalPos[startIndex], geneticPos[startIndex],
                                 physicalPos[endIndex], geneticPos[endIndex],
                                 queryPos);
        char buffer[50];
        sprintf(buffer, "chr%s:%d", chr.c_str(), queryPos);
        locName = buffer;
    }

    return success;
}

MapData::MapData(string filename, double mGap)
{
    ifstream fin;
    cerr << "Opening " << filename << "...\n";
    fin.open(filename.c_str());

    if (fin.fail())
    {
        cerr << "ERROR: Failed to open " << filename << " for reading.\n";
        throw 0;
    }

    int fileStart = fin.tellg();
    string line;
    int n = 0;
    int num_cols = 4;
    int current_cols = 0;
    while (getline(fin, line))
    {
        n++;
        current_cols = countFields(line);
        if (current_cols != num_cols)
        {
            cerr << "ERROR: line " << n << " of " << filename << " has " << current_cols
                 << ", but expected " << num_cols << ".\n";
            throw 0;
        }
    }

    fin.clear();
    fin.seekg(fileStart);

    cerr << "Loading map data for " << n << " loci.\n";

    this->initMapData(n+1);

    locusName[0] = "0";
    geneticPos[0] = 0;
    physicalPos[0] = 0;
    ppos2index[physicalPos[0]] = 0;


    for (int locus = 1; locus < nloci; locus++)
    {
        fin >> chr;
        fin >> locusName[locus];
        fin >> geneticPos[locus];
        fin >> physicalPos[locus];
        ppos2index[physicalPos[locus]] = locus;
    }

    MAXGAP = mGap;
    fin.close();
    return;
}

void MapData::initMapData(int n)
{
    if (n < 1)
    {
        cerr << "ERROR: number of loci (" << n << ") must be positive.\n";
        throw 0;
    }

    nloci = n;
    locusName = new string[nloci];
    physicalPos = new int[nloci];
    geneticPos = new double[nloci];

    for (int locus = 0; locus < nloci; locus++)
    {
        locusName[locus] = "--";
        physicalPos[locus] = MISSING;
        geneticPos[locus] = MISSING;
    }

    return;
}

MapData::~MapData()
{
    delete [] locusName;
    delete [] physicalPos;
    delete [] geneticPos;
}

int MapData::countFields(const string &str)
{
    string::const_iterator it;
    int result;
    int numFields = 0;
    int seenChar = 0;
    for (it = str.begin() ; it < str.end(); it++)
    {
        result = isspace(*it);
        if (result == 0 && seenChar == 0)
        {
            numFields++;
            seenChar = 1;
        }
        else if (result != 0)
        {
            seenChar = 0;
        }
    }
    return numFields;
}

