// N-Grams
// Code written by Jake Magid

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "map.h"
#include "vector.h"
#include "simpio.h"
using namespace std;

void Welcome() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups " << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}


Map<Vector<string>, Vector<string> > CreateInitialMap(ifstream &inputStream, int gramInt) {
    Map<Vector<string>, Vector<string> > ngramMap;
    Vector<string> mapWindow;
    Vector<string> mapValue;
    Vector<string> firstWindow;

    for (int i = 1; i < gramInt; i++) {
        string nextWord;
        inputStream >> nextWord;
        firstWindow.add(nextWord);
    }

    mapWindow = firstWindow;

    while (true) {
        string nextWord;
        inputStream >> nextWord;
        if (inputStream.fail()) break;
        if (ngramMap.containsKey(mapWindow)) {
            ngramMap[mapWindow] += nextWord;

        } else {
            mapValue.add(nextWord);
            ngramMap.add(mapWindow, mapValue);
            mapValue.clear();
        }
        mapWindow.remove(0);
        mapWindow.add(nextWord);
    }

    for (string testStr : firstWindow) {
        ngramMap[mapWindow] += testStr;
        mapWindow.remove(0);
        mapWindow.add(testStr);
    }

    inputStream.close();
    return ngramMap;
}


string GetRandomString(Vector<string> mapWindow) {
    int randInt = randomInteger(0, mapWindow.size() - 1);
    return mapWindow[randInt];
}


void PrintVectorStrings (Vector<string> compiledText) {
    for (string wordVal : compiledText) {
        cout << wordVal << " ";
    }
}


void GenerateText (int wordNum, Map<Vector<string>, Vector<string> > ngramMap) {
    Vector<Vector<string> > mapKeys = ngramMap.keys();
    int randInt = randomInteger(0, mapKeys.size() - 1);
    Vector<string> windowKey = mapKeys[randInt];
    int winSize = windowKey.size();

    Vector<string> compiledText;
    compiledText += windowKey;

    for (int i = 0; i < wordNum - winSize; i++) {
        string nextWord = GetRandomString(ngramMap[windowKey]);
        compiledText.add(nextWord);
        windowKey.remove(0);
        windowKey.add(nextWord);

    }

    cout << "... ";
    PrintVectorStrings(compiledText);
    cout << "..." << endl;

}


int GetWordNum() {
    int wordNum = 0;

    while (true) {
        cout << "# of random words to generate (0 to quit)? ";
        string wordStr;
        cin >> wordStr;

        bool Failed = false;
        for(string::iterator IT = wordStr.begin(); IT != wordStr.end(); IT++) {
            if (*IT < 48 || *IT > 57) {
                Failed = true;
                cout << "Illegal integer format. Try again." << endl;
                break;
            }
        }
        if(Failed == false) {
            wordNum = atoi(wordStr.c_str());
            return wordNum;
        }
    }

}


int main() {
    Welcome();

    ifstream inputStream;
    promptUserForFile(inputStream, "Input file name? ", "Unable to open that file. Try again. ");


    int gramInt = 0;

    while (true) {
        cout << "Value of N? ";
        string gramStr;
        cin >> gramStr;

        bool Failed = false;
        for(string::iterator IT = gramStr.begin(); IT != gramStr.end(); IT++) {
            if(*IT == 45 || *IT == 48 || *IT == 49) { //48
                Failed = true;
                cout << "N must be 2 or greater." << endl;
                break;
            } else if (*IT < 50 || *IT > 57) {
                Failed = true;
                cout << "Illegal integer format. Try again." << endl;
                break;
            }
        }
        if(Failed == false) {
            gramInt = atoi(gramStr.c_str());
            cout << endl;
            break;
        }
    }

    Map<Vector<string>, Vector<string> > ngramMap = CreateInitialMap(inputStream, gramInt);

    while (true) {
        int wordNum = GetWordNum();
        if (wordNum == 0) goto EXIT;

        while (wordNum < gramInt) {
            cout << "Must be at least " << gramInt << " words." << endl;
            cout << endl;
            wordNum = GetWordNum();
            if (wordNum == 0) goto EXIT;
        }
        GenerateText(wordNum, ngramMap);
        cout << endl;
    }


    EXIT:cout << "Exiting." << endl;
    return 0;
}
