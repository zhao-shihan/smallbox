#include "sbInterpolatingFunction.hh"

InterpolatingFunction::InterpolatingFunction(const vector<G4double>& x, const vector<G4double>& y) :
    data_(x, y) {
    if (data_.IsEmpty()) {
        G4ExceptionDescription error_message;
        error_message << "Data points do not exist." << G4endl;
        error_message << "The interpolating function will not properly functioned." << G4endl;
        G4Exception(
            "InterpolatingFunction::InterpolatingFunction(const vector<G4double>& x, const vector<G4double>& y)",
            "Invalid data", JustWarning, error_message);
    } else if (data_.Size() < 2) {
        G4ExceptionDescription error_message;
        error_message << "There is only one data point." << G4endl;
        error_message << "The interpolating function will not properly functioned." << G4endl;
        G4Exception(
            "InterpolatingFunction::InterpolatingFunction(const vector<G4double>& x, const vector<G4double>& y)",
            "Invalid data", JustWarning, error_message);
    }
}

InterpolatingFunction::InterpolatingFunction(const char csv_file_name[], const size_t& data_size) :
    data_(csv_file_name, data_size) {
    if (data_.IsEmpty()) {
        G4ExceptionDescription error_message;
        error_message << "Data points do not exist." << G4endl;
        error_message << "The interpolating function will not properly functioned." << G4endl;
        G4Exception(
            "InterpolatingFunction::InterpolatingFunction(const char csv_file_name[], const size_t& data_size)",
            "Invalid data", JustWarning, error_message);
    } else if (data_size < 2) {
        G4ExceptionDescription error_message;
        error_message << "There is only one data point." << G4endl;
        error_message << "The interpolating function will not properly functioned." << G4endl;
        G4Exception(
            "InterpolatingFunction::InterpolatingFunction(const char csv_file_name[], const size_t& data_size)",
            "Invalid data", JustWarning, error_message);
    }
}

