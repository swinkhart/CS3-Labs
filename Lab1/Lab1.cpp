//Finds the differences in two files
//Lab1
//Seth Winkhart
//1/18/2022

#include <iostream>
#include <fstream>
#include <string>

//takes three string and returns the length of the file line + the header that
//prints before it
int checkDiff(std::string x, std::string y, std::string h)
{
    int carrotTrack = 0;

    //compares the second line to the first line to find the first difference
    for (int i = 0; i <= y.length(); ++i)
    {
        if (y[i] != x[i])
        {
            break;
        }
        else
        {
            ++carrotTrack;
        }
    }
    return carrotTrack + h.length();
}

int main(int argc, char *argv[])
{
    //check if there are more than two arguments
    if (argc == 3)
    {
        //declare strings and get files
        std::string file1Txt, file2Txt, spaceString, fileName, header;
        std::ifstream file1(argv[1]);
        std::ifstream file2(argv[2]);

        //get file names, only one since they are the same length
        fileName = argv[1];

        int lineNum = 1;

        //reads entire files, line by line until end of file is reached
        while (!file1.eof() || !file2.eof())
        {
            if (file1.eof())
            {
                //includes empty lines in first file
                file1Txt = "";
            }
            else
            {
                getline(file1, file1Txt);
            }

            if (file2.eof())
            {
                //includes empty lines in second file
                file2Txt = "";
            }
            else
            {
                getline(file2, file2Txt);
            }

            //checks to see of there is any differnece in the current lines
            //if there is none then it moves to the next one
            if (file1Txt != file2Txt)
            {
                //combines all the formatting into one string to find the
                //length to accuratly print the carrot
                header = fileName + ": " + std::to_string(lineNum) + ": ";

                //print out text in files
                std::cout << argv[1] << ": " << lineNum << ": " << file1Txt << std::endl;
                std::cout << argv[2] << ": " << lineNum << ": " << file2Txt << std::endl;

                //prints the carrot using the int checkDiff returns as the
                //length of spcaces to print before the carrot
                std::cout << spaceString.assign(checkDiff(file1Txt, file2Txt, header), ' ') << '^' << std::endl;
                std::cout << '\n';
            }

            ++lineNum;
        }

        file1.close();
        file2.close();
    }
    else
    {
        //if there are more than two arguments this will print
        std::cout << "error: cannot invoke with more than two arguments" << '\n';
    }
}
