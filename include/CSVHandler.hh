#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H 1

#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

// Read a csv file in this format:
// 
// #[Annotation]
// [KEY1],[data1],[data2],...,[dataN1],#[Annotation]
// [KEY2],[data1],[data2],...,[dataN2]
// #[Annotation]
// [KEY3],[data1],[data2],...,[dataN3]
// ...
//
// e.g.
// #refraction property
// RINDEX,1.1,1.2,1.3,#refraction index
// ENERGY,1.5,1.6,1.7,#energy
//
// Special case:
// (empty line)                             : Skip.
// ,[data1],[data2],...,[dataN]             : Will skip if no keyword.
// ,                                        : Same as above.
// ,#[Annotation]                           : Same as above.
// [KEY],                                   : Create a map with empty vector.
// [KEY],[data1],,[data3],...               : Null data is treated as 0.
// [KEY],[data1],(not-a-number),[data3],... : String that are not numbers are treated as 0.
// A,[data1],[data2],...,[dataN1]
// A,[data1],[data2],...,[dataN2]           : For data with the same keyword, keep the first data.
template<typename key_t, typename data_t>
std::map<key_t, std::vector<data_t>> CreateMapFromCSV(const std::string& csvFileName) {
    using std::map;
    using std::string;
    using std::vector;

    std::ifstream fin(csvFileName);
    if (!fin.is_open()) {
        std::cerr << '<' << csvFileName << "> not found. Empty map will be created." << std::endl;
        return map<key_t, vector<data_t>>();
    }

    map<key_t, vector<data_t>> dataMap;

    string line;
    while (!fin.eof()) {
        getline(fin, line);
        if (line.empty()) { continue; }  // Skip if empty.
        size_t annotationPos = line.find_first_of('#');
        if (annotationPos != string::npos) {  // If there are annotation,
            line.erase(annotationPos); // drop annotation.
            if (annotationPos == 0) {
                continue;
            } else if (line[annotationPos - 1] == ',') {  // If there is a comma before annotation,
                line.erase(annotationPos - 1);  // drop this comma.
            }
        }
        if (line.empty()) { continue; }  // Skip if there is only annotation.

        size_t nextCommaPos = line.find_first_of(',');
        if (nextCommaPos == 0) { continue; } // If there is no key, skip.
        std::stringstream ss;
        key_t key; // The key.
        ss << line.substr(0, nextCommaPos);
        ss >> key;
        if (nextCommaPos == string::npos) {  // If there is no data,
            dataMap.insert({ key,{} });  // create a map without data.
            continue;
        }

        vector<data_t> data;
        size_t lastCommaPos;
        do {
            lastCommaPos = nextCommaPos;
            nextCommaPos = line.find(',', lastCommaPos + 1);
            ss.clear();
            data_t property;
            ss << line.substr(lastCommaPos + 1, nextCommaPos - lastCommaPos - 1);
            ss >> property;
            data.push_back(property);
        } while (nextCommaPos != string::npos);

        dataMap.insert({ key,data });
    }
    fin.close();
    return dataMap;
}

template<typename key_t, typename data_t>
void CreateCSVFromMap(const std::map<key_t, std::vector<data_t>>& dataMap, const std::string& csvFileName) {
    using std::map;
    using std::string;
    using std::vector;

    std::ofstream fout(csvFileName);
    if (!fout.is_open()) {
        std::cerr << "Cannot open or create <" << csvFileName << ">." << std::endl;
        return;
    }

    for (const auto& keyDataPair : dataMap) {
        fout << keyDataPair.first;
        for (const auto& data : keyDataPair.second) {
            fout << ',' << data;
        }
        fout << std::endl;
    }

    fout.close();
}

#endif

