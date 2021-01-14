#include "sbInterpolatingFunction.hh"

sbInterpolatingFunction::sbInterpolatingFunction(const char file_name[], size_t size) :
    data_(size) {
    std::ifstream fin(file_name);
    if (fin.is_open()) {
        for (auto xy_pair = data_.begin(); xy_pair != data_.end(); ++xy_pair) {
            fin >> xy_pair->first;
            fin.get();
            fin >> xy_pair->second;
            fin.get();
        }
        fin.close();
    }
}

// ToDo: sbInterpolatingFunction::sbInterpolatingFunction(const vector<G4double>& x, const vector<G4double>& y) {}

sbInterpolatingFunction::~sbInterpolatingFunction() {}

inline G4double sbInterpolatingFunction::Interpolating(const G4double& x) const {
    // ToDo
}

