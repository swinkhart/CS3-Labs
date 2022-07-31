//reads in file and uses the wordCount class to count the number of words
//Seth Winkhart
//1/31/2022

#include "wordCount.h"

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        std::string fileTxt, formattedStr;
        std::ifstream file1(argv[1]);

        //create wordList
        WordList w;

        //read until end of file
        while (!file1.eof())
        {
            if (file1.eof())
            {
                //includes empty lines in first file
                fileTxt = "";
            }
            else
            {
                //read in file word by word
                file1 >> fileTxt;

                //turn each char of the word into lower case and check if the
                //char is an alphanumeric char, if it is then save it
                for (int i = 0; i < fileTxt.size(); ++i)
                {
                    char nextChar = tolower(fileTxt[i]);
                    if (isalnum(nextChar))
                    {
                        formattedStr.push_back(nextChar);
                    }
                }
            }

            //add the formatted word to the WordList and reset the formatted
            //word string for the next word
            w.addWord(formattedStr);
            formattedStr = "";
        }

        w.print();
    }
    else
    {
        std::cout << "error: more than two arguments" << std::endl;
    }
}
