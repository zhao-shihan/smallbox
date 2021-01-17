#ifndef SB_INTERPOLATING_FUNCTION_H
#define SB_INTERPOLATING_FUNCTION_H 1

#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>

#include "globals.hh"

using std::vector;

class XYpair : public std::pair<G4double, G4double> {
public:
    bool operator>(const XYpair& rhs) { return this->first > rhs.first; }
    bool operator<(const XYpair& rhs) { return this->first < rhs.first; }
    bool operator>=(const XYpair& rhs) { return this->first >= rhs.first; }
    bool operator<=(const XYpair& rhs) { return this->first <= rhs.first; }
};

#ifndef SBIF_ENABLE_AUTO_EXTRAPOLATION
#define SBIF_ENABLE_AUTO_EXTRAPOLATION 0
#endif

class sbInterpolatingFunction {
private:
    vector<XYpair> data_;

public:
    sbInterpolatingFunction(const char csv_file_name[], const size_t& data_size);
    sbInterpolatingFunction(const vector<XYpair>& data);
    ~sbInterpolatingFunction() {}

    inline G4double operator()(const G4double& x) const {
#if SBIF_ENABLE_AUTO_EXTRAPOLATION
        if (x <= data_.begin()->first) {
            const vector<XYpair>::const_iterator it_1 = data_.begin();
            const vector<XYpair>::const_iterator it_2 = it_1 + 1;
            return it_1->second +
                (it_2->second - it_1->second) / (it_2->first - it_1->first) * (x - it_1->first);
        }
        if (x >= data_.rbegin()->first) {
            const vector<XYpair>::const_reverse_iterator rit_2 = data_.rbegin();
            const vector<XYpair>::const_reverse_iterator rit_1 = rit_2 + 1;
            return rit_2->second +
                (rit_2->second - rit_1->second) / (rit_2->first - rit_1->first) * (x - rit_2->first);
        }
#endif
        vector<XYpair>::const_iterator it_1 = data_.begin();
        vector<XYpair>::const_iterator it_2 = data_.end() - 1;
        std::ptrdiff_t diff = it_2 - it_1;
        std::ptrdiff_t step = diff / 2;
        while (step > 0) {
            if ((it_1 + step)->first < x) {
                it_1 += step;
            } else {
                it_2 -= step;
            }
            diff -= step;
            step = diff / 2;
        }
        return it_1->second +
            (it_2->second - it_1->second) / (it_2->first - it_1->first) * (x - it_1->first);
    }
};

#endif

