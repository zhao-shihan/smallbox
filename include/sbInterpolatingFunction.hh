#ifndef SB_INTERPOLATING_FUNCTION_H
#define SB_INTERPOLATING_FUNCTION_H 1

#include "sbXYlist.hh"

class InterpolatingFunction {
private:
    XYlist data_;

public:
    InterpolatingFunction(const vector<G4double>& x, const vector<G4double>& y);
    InterpolatingFunction(const char csv_file_name[], const size_t& data_size);
    ~InterpolatingFunction() {}

    inline G4double operator()(const G4double& x) const;
};

#ifndef SBIF_ENABLE_AUTO_EXTRAPOLATION
#define SBIF_ENABLE_AUTO_EXTRAPOLATION 0
#endif
inline G4double InterpolatingFunction::operator()(const G4double& x) const {
#if SBIF_ENABLE_AUTO_EXTRAPOLATION
    if (x <= *(data_.cx().begin())) {
        vector<G4double>::const_iterator it_x_1 = data_.cx().begin();
        vector<G4double>::const_iterator it_x_2 = it_x_1 + 1;
        vector<G4double>::const_iterator it_y_1 = data_.cy().begin();
        vector<G4double>::const_iterator it_y_2 = it_y_1 + 1;
        return *it_y_1 + (*it_y_2 - *it_y_1) / (*it_x_2 - *it_x_1) * (x - *it_x_1);
    }
    if (x >= *(data_.cx().rbegin())) {
        vector<G4double>::const_reverse_iterator it_x_2 = data_.cx().rbegin();
        vector<G4double>::const_reverse_iterator it_x_1 = it_x_2 + 1;
        vector<G4double>::const_reverse_iterator it_y_2 = data_.cy().rbegin();
        vector<G4double>::const_reverse_iterator it_y_1 = it_y_2 + 1;
        return *it_y_2 + (*it_y_2 - *it_y_1) / (*it_x_2 - *it_x_1) * (x - *it_x_2);
    }
#endif
    vector<G4double>::const_iterator it_x_1 = data_.cx().begin();
    vector<G4double>::const_iterator it_x_2 = data_.cx().end() - 1;
    std::ptrdiff_t diff = it_x_2 - it_x_1;
    std::ptrdiff_t step = diff / 2;
    while (step > 0) {
        if (*(it_x_1 + step) < x) {
            it_x_1 += step;
        } else {
            it_x_2 -= step;
        }
        diff -= step;
        step = diff / 2;
    }
    vector<G4double>::const_iterator it_y_1 = data_.cy().begin() + (it_x_1 - data_.cx().begin());
    vector<G4double>::const_iterator it_y_2 = it_y_1 + 1;
    return *it_y_1 + (*it_y_2 - *it_y_1) / (*it_x_2 - *it_x_1) * (x - *it_x_1);
}

#endif

