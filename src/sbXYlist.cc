#include "sbXYlist.hh"

XYlist::XYlist(const vector<G4double>& x, const vector<G4double>& y) {
    if (x.size() == y.size()) {
        xylist_.first = x;
        xylist_.second = y;
    } else {
        G4ExceptionDescription error_message;
        error_message << "x, y are not the same length." << G4endl;
        error_message << "This XYlist will stay empty." << G4endl;
        G4Exception("XYlist::XYlist(const vector<G4double>& x, const vector<G4double>& y)", "Length mismatches",
            JustWarning, error_message);
    }
}

XYlist::XYlist(const char csv_file_name[], const size_t& data_size) {
    std::ifstream fin(csv_file_name);
    if (fin.is_open()) {
        xylist_.first.resize(data_size);
        xylist_.second.resize(data_size);
        for (size_t i = 0; i < data_size; ++i) {
            fin >> xylist_.first[i];
            fin.get();
            fin >> xylist_.second[i];
            fin.get();
        }
        fin.close();
    } else {
        G4ExceptionDescription error_message;
        error_message << "Data file:" << G4endl;
        error_message << csv_file_name << G4endl;
        error_message << "not found." << G4endl;
        error_message << "This XYlist will stay empty." << G4endl;
        G4Exception("XYlist::XYlist(const char csv_file_name[], const size_t& data_size)",
            "Data file not found", JustWarning, error_message);
    }
}

void XYlist::SetXY(const vector<G4double> x, const vector<G4double> y) {
    if (x.size() == y.size()) {
        xylist_.first = x;
        xylist_.second = y;
    } else {
        G4ExceptionDescription error_message;
        error_message << "New x, y are not the same length." << G4endl;
        error_message << "This XYlist will stay unchanged." << G4endl;
        G4Exception("XYlist::SetXY(const vector<G4double> x, const vector<G4double> y)", "Length mismatches",
            JustWarning, error_message);
    }
}

void XYlist::SetX(const vector<G4double> x) {
    if (x.size() == xylist_.second.size()) {
        xylist_.first = x;
    } else {
        G4ExceptionDescription error_message;
        error_message << "The new x is not the same length as current y." << G4endl;
        error_message << "This XYlist will stay unchanged." << G4endl;
        G4Exception("XYlist::SetX(const vector<G4double> x)", "Length mismatches", JustWarning, error_message);
    }
}

void XYlist::SetY(const vector<G4double> y) {
    if (y.size() == xylist_.first.size()) {
        xylist_.second = y;
    } else {
        G4ExceptionDescription error_message;
        error_message << "The new y is not the same length as current x." << G4endl;
        error_message << "This XYlist will stay unchanged." << G4endl;
        G4Exception("XYlist::SetY(const vector<G4double> y)", "Length mismatches", JustWarning, error_message);
    }
}

