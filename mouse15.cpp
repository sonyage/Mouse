//
// mouse15.cpp
//
// Sonya Gomez Enriquez
// 10/5/2019
// Assignment 3
// This assignment will move a mouse around an island 100 times and gather
// statistics

#include <iostream>
#include <time.h>
#include <fstream>
#include <iomanip>
using namespace std;

void moveMouse(int &mouseR, int &mouseC);
void copyMap(int mapCopy[][20], int row, int column, ifstream& fin);
void readMap(int &mouseR, int &mouseC, int& bridge1R, int& bridge1C,
             int& bridge2R, int& bridge2C, int mapCopy[][20], int row,
             int column);
void printMap(int mapCopy[][20], ofstream& fout, int row, int column);
int largestElement(int mapCopy[][20], int row, int column);
void printMessage(int lives, bool drowned, bool escaped, bool starved,
                  ofstream& fout);

const int WATER = -1;
const int LAND = 0;
const int MOUSE = 1;

int main(int argc, char* argv[])
{
    int row, column, lives = 0, moves = 0,
    starves = 0, escapes = 0, drowns = 0;
    int mouseR, mouseC, mStartR, mStartC, bridge1R, bridge1C, bridge2R,
        bridge2C;
    ifstream fin; ofstream fout;
    int mapCopy[20][20] = {0}, countMap[20][20] = {0};
    bool timeSeed = false, variableSeed = false;
    bool drowned = false, escaped = false, starved = false;

    // parse command line parameters, open files
    if(argc != 4)
    {
        cout << "Error in program call." << endl;
    }
    else
    {
        fin.open(argv[1]);
        fout.open(argv[2]);

        if(!fin)
        {
            cout << "Error opening files." << endl;
        }
        else
        {
            //seed time
            if(atoi(argv[3]) == -1)
            {
                srand(time(nullptr)); // if negative 1, seed w/ time
                timeSeed = true;
            }
            else if(atoi(argv[3]) > 0)
            {
                srand(atoi(argv[3])); // if a positive #, seed with the #
                variableSeed = true;
            }
            // if argv[3] is 0, it will not be seeded


            fin >> row >> column;
            fin.ignore(); // ignore new line

            copyMap(mapCopy, row, column, fin);
            readMap(mStartR, mStartC, bridge1R, bridge1C, bridge2R, bridge2C,
                    mapCopy, row, column);
            mouseR = mStartR;
            mouseC = mStartC;

            fout << "The input file is: " << argv[1] << endl
                 << "The output file is: " << argv[2] << endl;

            fout << "\nMap:" << endl;
            printMap(mapCopy, fout, row, column);

            fout << "\nRand in the program was ";
            if(timeSeed)
            {
                fout << "seeded with time." << endl;
            }
            else if(variableSeed)
            {
                fout << "seeded with the number " << argv[3] << "." << endl;
            }
            else
            {
                fout << "not seeded." << endl;
            }

            fout << "The mouse's initial position is at [" << mStartR << "]["
                 << mStartC << "]." << endl
                 << "The first bridge is at [" << bridge1R << "]["
                 << bridge1C << "]." << endl
                 << "The second bridge is at [" << bridge2R << "]["
                 << bridge2C << "]." << endl;

            fout << "\nHere begins the life runs:" << endl;
            while(lives < 100)
            {
                moves = 0; // reset
                escaped = drowned = starved = false;
                mouseR = mStartR;
                mouseC = mStartC;

                while(moves < 100 && !drowned && !escaped)
                {
                    moveMouse(mouseR, mouseC);
                    countMap[mouseR][mouseC]++;

                    if(mapCopy[mouseR][mouseC] == WATER)
                    {
                        drowned = true;
                        drowns++;
                    }

                    if( ( (mouseR == bridge1R) && (mouseC == bridge1C) ) ||
                       ( (mouseR == bridge2R) && (mouseC == bridge2C) ) )
                    {
                        escaped = true;
                        escapes++;
                    }
                    moves++;
                }

                if(moves >= 100)
                {
                    starves++;
                    starved = true;
                }

                lives++;
                printMessage(lives, drowned, escaped, starved, fout);
                fout << endl;
            }
            // statistics
            fout << "\nStatistics:" << endl
                 << "The mouse escaped " << escapes << " times." << endl
                 << "The mouse drowned " << drowns << " times." << endl
                 << "The mouse starved " << starves << " times." << endl
                 << "\nCount Map of Mouse Moves:" << endl;
                 printMap(countMap, fout, row, column);

            fin.close();
            fout.close();
            return -1;
        }
    }
}

void moveMouse(int &mouseR, int &mouseC)
{
    int r = rand() % 4;
    static int moves[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    mouseR += moves[r][0];
    mouseC += moves[r][1];
    return;
}

void copyMap(int mapCopy[][20], int row, int column, ifstream& fin)
{
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            fin >> mapCopy[i][j];
        }
        fin.ignore(); // ignore new line @ end of every row
    }
    return;
}

void readMap(int &mouseR, int &mouseC, int& bridge1R, int& bridge1C,
             int& bridge2R, int& bridge2C, int mapCopy[][20], int row,
             int column)
{
    int i = 0, j = 0;
    bool bridge1Found = false, bridge2Found = false;

    for(i = 0; i < row; i++) // loop through every element
    {
        for(j = 0; j < column; j++)
        {
            if(mapCopy[i][j] == 1 ) // 1 is the mouses location
            {
                mouseR = i;
                mouseC = j;
                mapCopy[i][j] = LAND; // change mouse map position to LAND
            }
        }
    }
    //find bridge 1
    while(!bridge1Found)
    {
        // check left border top to bottom
        for(i = 0; i < row && !bridge1Found; i++)
        {
            if(mapCopy[i][0] == 0)
            {
                bridge1R = i;
                bridge1C = 0;
                bridge1Found = true;
            }
        }
        //check bottom border left to right
        for(i = 0; i < column && !bridge1Found; i++)
        {
            if(mapCopy[row - 1][i] == 0)
            {
                bridge1R = row - 1;
                bridge1C = i;
                bridge1Found = true;
            }
        }
        // check right border bottom to top
        for(i = row - 1; i >= 0 && !bridge1Found; i--)
        {
            if(mapCopy[i][column - 1] == 0)
            {
                bridge1R = i;
                bridge1C = column - 1;
                bridge1Found = true;
            }
        }
        // check top border right to left
        for(i = column - 1; i >= 0 && !bridge1Found; i--)
        {
            if(mapCopy[0][i] == 0)
            {
                bridge1R = 0;
                bridge1C = i;
                bridge1Found = true;
            }
        }

    }
    while(!bridge2Found)
    {
        // check top border right to left
        for(i = 0; i < column && !bridge2Found; i++)
        {
            if(mapCopy[0][i] == 0)
            {
                bridge2R = 0;
                bridge2C = i;
                bridge2Found = true;
            }
        }
        // check right border top to bottom
        for(i = 0; i < row && !bridge2Found; i++)
        {
            if(mapCopy[i][column - 1] == 0)
            {
                bridge2R = i;
                bridge2C = column - 1;
                bridge2Found = true;
            }
        }
        //check bottom border left to right
        for(i = column - 1; i >= 0 && !bridge2Found; i--)
        {
            if(mapCopy[row - 1][i] == 0)
            {
                bridge2R = row - 1;
                bridge2C = i;
                bridge2Found = true;
            }
        }
        // check left border bottom to top
        for(i = row - 1; i >= 0 && !bridge2Found; i--)
        {
            if(mapCopy[i][0] == 0)
            {
                bridge2R = i;
                bridge2C = 0;
                bridge2Found = true;
            }
        }
    }
    return;
}

void printMap(int mapCopy[][20], ofstream& fout, int row, int column)
{
    int width = 2, i, j;
    int largest = largestElement(mapCopy, row, column);
    while(largest /= 10)
    {
        width++;
    }

    for(i = 0; i < row; i++)
    {
        for(j = 0; j < column; j++)
        {
            fout << setw(width) << mapCopy[i][j];
        }
        fout << endl;
    }
    return;
}

int largestElement(int mapCopy[][20], int row, int column)
{
    int largest = 0, i, j;
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < column; j++)
        {
            if(mapCopy[i][j] > largest)
            {
                largest = mapCopy[i][j];
            }
        }
    }
    return largest;
}

void printMessage(int lives, bool drowned, bool escaped, bool starved,
                  ofstream& fout)
{
    if(drowned)
    {
        fout << "Uh Oh! The mouse drowned on life " << lives << "!";
    }
    else if(escaped)
    {
        fout << "This is a miracle! The mouse escaped on life " << lives
                 << "!";
    }
    else if(starved)
    {
        fout << "Unfortunately, the mouse failed to escape and starved on life "
             << lives << ".";
    }
    return;
}
