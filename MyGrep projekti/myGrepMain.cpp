//Made by Kalle Laaksonen 25TietoA
//MyGrep-project 

//Included libraries
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace std;

//=====================Functions==========================

// Function to convert characters to lower case
string toLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return tolower(c); });
    return result;
}

//Function to find pattern from given text
int findSubstring(const string& text,
    const string& pattern,
    bool ignoreCase)
{
    if (pattern.empty()) return 0;

    if (ignoreCase) {
        string t = toLower(text);
        string p = toLower(pattern);
        size_t pos = t.find(p);
        return (pos == string::npos) ? -1 : (int)pos;
    }
    else {
        size_t pos = text.find(pattern);
        return (pos == string::npos) ? -1 : (int)pos;
    }
}

// Function to concert file to lines
vector<string> readFileLines(const string& filename)
{
    ifstream file(filename);
    if (!file) {
		throw runtime_error("Could not open file \"" + filename + "\""); //Error for file opening
    }

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

// Retuns the line numbers of lines that match the pattern
vector<int> searchLines(
    const vector<string>& lines,
    const string& pattern,
    bool ignoreCase,
    bool reverseSearch)
{
    vector<int> result;

    for (size_t i = 0; i < lines.size(); ++i) {
        bool found = (findSubstring(lines[i], pattern, ignoreCase) >= 0);

        if ((!reverseSearch && found) ||
            (reverseSearch && !found)) {
            result.push_back((int)i + 1);
        }
    }
    return result;
}

// Function to print the matching lines
void printResults(const vector<string>& lines,
    const vector<int>& matchingLines,
    bool printLineNumbers)
{
    for (int ln : matchingLines) {
        if (printLineNumbers) {
            cout << ln << ":";
        }
        cout << lines[ln - 1] << endl;
    }
}

// Function to get the size of the file
long getFileSize(const string& filename)
{
    ifstream file(filename, ios::binary | ios::ate);
    if (!file) {
        throw runtime_error(
			"Could not find out the size of file \"" + filename + "\""); //Error for file size
    }
    return file.tellg();
}


//=======================Main=====================

int main(int argc, char* argv[])
{
    try {
        
        if (argc == 1) {
            string bigString, pattern;

            cout << "Give a string from which to search for: ";
            getline(cin, bigString);

            cout << "Give search string: ";
            getline(cin, pattern);

            int pos = findSubstring(bigString, pattern, false);

            if (pos >= 0) {
                cout << "\"" << pattern << "\" found in \""
                    << bigString << "\" in position " << pos << endl;
            }
            else {
                cout << "\"" << pattern << "\" NOT found in \""
                    << bigString << "\"" << endl;
            }
            return 0;
        }

		//Command arguments

        bool printLineNumbers = false;   // -l command
        bool countOccurrences = false;   // -o command
        bool reverseSearch = false;      // -r command 
        bool ignoreCase = false;         // -i command

        string options;
        string pattern;
        string filename;

		//Check if there are arguments
        if (argv[1][0] == '-') {
            options = argv[1];
            pattern = argv[2];
            filename = argv[3];
        }
        else {
            pattern = argv[1];
            filename = argv[2];
        }

		// Íf there is arguments, check which options are given
        if (options.find('l') != string::npos) printLineNumbers = true;
        if (options.find('o') != string::npos) countOccurrences = true;
        if (options.find('r') != string::npos) reverseSearch = true;
        if (options.find('i') != string::npos) ignoreCase = true;

		// Get the file size, for testing if file exists and is readable
        long size = getFileSize(filename);
        (void)size;

        //Read file lines
        vector<string> lines = readFileLines(filename);

        // Search lines for pattern
        vector<int> matches =
            searchLines(lines, pattern, ignoreCase, reverseSearch);

        // Print matching lines
        printResults(lines, matches, printLineNumbers);

		// If -o option is given, print the number of occurrences
        if (countOccurrences) {
            if (reverseSearch) {
                cout << "\nOccurrences of lines NOT containing \""
                    << pattern << "\": " << matches.size() << endl;
            }
            else {
                cout << "\nOccurrences of lines containing \""
                    << pattern << "\": " << matches.size() << endl;
            }
        }

	}// Catch any exceptions that may occur and print the error message
    catch (const exception& e) {
        cout << "An exception occurred. Exception Nr. -1\n";
        cout << e.what() << endl;
    }

    return 0;
}
