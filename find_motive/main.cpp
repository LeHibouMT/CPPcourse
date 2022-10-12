// Exercice réalisé par Michel Taing avec Jeanne Boulet 
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

bool motiveinString(string s, string motive)
{
    if (s.size() < motive.size())
    {
        return false;
    }
    for (int i = 0; i <= s.size() - motive.size(); i++)
    {
        bool test = true;
        for (int j = 0; j < motive.size(); j++)
        {
            if (!(s[i + j] == motive[j]))
            {
                test = false;
            }
        }
        if (test)
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[])
{
    int count = 0;
    string file = argv[1];
    string m = argv[2]; // m est le motif
    fstream myfile; // déclaration du fichier
    string line; // déclaration de ligne
    myfile.open(file); // ouverture du fichier
    if (myfile.is_open())
    {
        while (myfile >> line)
        {
            //cout << line << endl;
            if (motiveinString(line,m))
            {
                count++;
            }
            //cout << count << endl;
        }
        myfile.close();
        cout << "The file " << file << " contains " << count << " words containing the motive " << m << endl ;
        return 1;
    }
    else
    {
        cout << "The file " << file << " could not be opened.";
        return 0;
    }
}

