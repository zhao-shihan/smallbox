#ifndef SB_INTERPOLATING_FUNCTION
#define SB_INTERPOLATING_FUNCTION 1

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

class sbInterpolatingFunction {
private:
    vector<XYpair> data_;

public:
    sbInterpolatingFunction(const char file_name[], size_t size);
    // ToDo: sbInterpolatingFunction(const vector<G4double>& x, const vector<G4double>& y);
    ~sbInterpolatingFunction() {}
    inline G4double operator()(const G4double& x) const {
        size_t _1 = 0;
        size_t _2 = data_.size() - 1;
        size_t step = 0;
        do {
            step = (_2 - _1) / 2;
            if (data_[_1 + step].first < x) {
                _1 += step;
            } else {
                _2 -= step;
            }
        } while (_2 - _1 > 1);
        return data_[_1].second +
            (data_[_2].second - data_[_1].second) / (data_[_2].first - data_[_1].first) * (x - data_[_1].first);
    }
};

#endif

