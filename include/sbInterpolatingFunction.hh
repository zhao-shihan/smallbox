#ifndef SB_INTERPOLATING_FUNCTION
#define SB_INTERPOLATING_FUNCTION 1

#include <vector>
#include <utility>
#include <fstream>

#include "globals.hh"

using std::vector;
using std::pair;

class sbInterpolatingFunction {
private:
    vector<pair<G4double, G4double>> data_;

public:
    sbInterpolatingFunction(const char file_name[], size_t size);
    // ToDo: sbInterpolatingFunction(const vector<G4double>& x, const vector<G4double>& y);
    ~sbInterpolatingFunction();
    inline G4double Interpolating(const G4double& x) const;
};

#endif

