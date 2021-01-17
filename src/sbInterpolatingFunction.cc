#include "sbInterpolatingFunction.hh"

sbInterpolatingFunction::sbInterpolatingFunction(const char file_name[], const size_t& size) :
    data_(size) {
    if (size < 2) {
        G4ExceptionDescription error_message;
        error_message << "Interpolating points number is less than 2." << G4endl;
        G4Exception("sbInterpolatingFunction::sbInterpolatingFunction()", "Invalid data",
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
        error_message << "Data file:" << G4endl;
        error_message << file_name << G4endl;
        error_message << "not found." << G4endl;
        G4Exception("sbInterpolatingFunction::sbInterpolatingFunction()", "Data file not found",
            FatalException, error_message);
    }
}

sbInterpolatingFunction::sbInterpolatingFunction(const vector<XYpair>& data) :
    data_(data) {
    if (data_.size() < 2) {
        G4ExceptionDescription error_message;
        error_message << "Interpolating points number is less than 2.\n";
        G4Exception("sbInterpolatingFunction::sbInterpolatingFunction()", "Invalid data",
            FatalException, error_message);
    }
    std::sort(data_.begin(), data_.end());
}

