#include "sbInterpolatingFunction.hh"

sbInterpolatingFunction::sbInterpolatingFunction(const char file_name[], size_t size) :
    data_(size) {
    if (size < 2) {
        G4ExceptionDescription error_message;
        error_message << "Interpolating points number is less than 2.\n";
        G4Exception("sbInterpolatingFunction::sbInterpolatingFunction()", "sbIF001",
            FatalException, error_message);
    }
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
        G4Exception("sbInterpolatingFunction::sbInterpolatingFunction()", "sbIF002",
            FatalException, error_message);
    }
}

sbInterpolatingFunction::sbInterpolatingFunction(const vector<XYpair> data) :
    data_(data) {
    if (data_.size() < 2) {
        G4ExceptionDescription error_message;
        error_message << "Interpolating points number is less than 2.\n";
        G4Exception("sbInterpolatingFunction::sbInterpolatingFunction()", "sbIF001",
            FatalException, error_message);
    }
    std::sort(data_.begin(), data_.end());
}

