#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "coordinates.h"
#include <locale>
#include <codecvt>

using namespace std;

void readCSV(const string& filename, vector<std::wstring>& col1, vector<Coordinates> & col2) {
    std::locale german_locale("de_DE.UTF-8");
    std::locale::global(german_locale);
    std::locale russian_locale("ru_RU.UTF-8");
    std::locale::global(russian_locale);
    wifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    wstring line;
    wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    while (getline(file, line)) {
        wstringstream ss(line);
        wstring wcell;
        
        getline(ss, wcell, L',');
        col1.push_back(wcell);

        wstring wcell1;
        getline(ss, wcell1, L',');
        wstring wcell2;
        std::getline(ss, wcell2, L',');
        string cell1 = converter.to_bytes(wcell1);
        string cell2 = converter.to_bytes(wcell2);
        cell1[2] = ',';
        cell2[2] = ','; 
        double num1 = std::stod(cell1);
        double num2 = std::stod(cell2);
        col2.push_back({num1,num2});
    }

    file.close();
}
 
