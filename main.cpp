#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <map>

using namespace std;

string readFile(const char s[]){

    ifstream file(s);

    stringstream input;

    if(file){

        input <<file.rdbuf();

        file.close();

    }

    return input.str();

}

template <typename Iterator>
Iterator compressText(const string &uncompressed, Iterator result, int maxSize) {

    int dictSize = 256;
    map<string,int> dictionary;

    for (int i = 0; i < 256; i++)
        dictionary[string(1, i)] = i; // 256 ascii values

    string t;

    for (string::const_iterator it = uncompressed.begin();
         it != uncompressed.end(); ++it) {

        char c = *it;

        string tc = t + c;

        if (dictionary.count(tc)){
            t = tc;
        }
        else {
            *result++ = dictionary[t];
            dictionary[tc] = dictSize++;
            t = string(1, c);
        }

    }

  if (!t.empty()){
    *result++ = dictionary[t];
  }

  return result;

}

template <typename Iterator>
string decompressText(Iterator begin, Iterator end, int maxSize) {

    int dictSize = 256;

    map<int,string> dictionary;

    for (int i = 0; i < 256; i++)
        dictionary[i] = string(1, i);

    string w(1, *begin++);
    string result = w;
    string entry;

    for (; begin != end; begin++) {

        int k = *begin;
        if (dictionary.count(k))
            entry = dictionary[k];
        else if (k == dictSize)
            entry = w + w[0];

        result += entry;

        dictionary[dictSize++] = w + entry[0];

        w = entry;

    }

    return result;

}


int main(int argc, const char* argv[]) {

    if(argc != 4)
        return 0;

    const char* choice = argv[1];

    int maxSize = stoi(argv[2]);

    vector<int> compressed;

    if(*choice == 'c'){

        compressed.clear();

        ofstream out("compressed.txt");

        compressText(readFile(argv[3]), back_inserter(compressed), maxSize);

            for(size_t i  = 0; i < compressed.size(); i++){

                if(i == compressed.size() - 1){
                    out<< compressed.at(i);
                    break;
                }

                out<< compressed.at(i) << " ";
            }

    }


    else if(*choice == 'd'){

        compressed.clear();

        ofstream out("decompressed.txt");

        ifstream input(argv[3]);

        int x;

            while(!input.eof()){
                input>>x;
                compressed.push_back(x);
            }

        input.close();

        string decompressed = decompressText(compressed.begin(), compressed.end(), maxSize);

        out<<decompressed;

    }

  return 0;

}
