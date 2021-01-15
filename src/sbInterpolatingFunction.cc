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
        std::sort(data_.begin(), data_.end());
    } else {
        G4ExceptionDescription error_message;
        error_message << file_name << "not found.\n";
        G4Exception("sbInterpolatingFunction::sbInterpolatingFunction()", "MyCode0002",
            FatalException, error_message);
    }
}

// ToDo: sbInterpolatingFunction::sbInterpolatingFunction(const vector<G4double>& x, const vector<G4double>& y) {}

