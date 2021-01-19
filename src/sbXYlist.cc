#include "sbXYlist.hh"

XYlist::XYlist(const vector<G4double>& x, const vector<G4double>& y) {
    if (x.size() == y.size()) {
        xylist_.first = x;
        xylist_.second = y;
    } else {
        ThrowLengthMismatches(
            "x, y are not the same length.\nThis XYlist will stay empty.",
            "XYlist::XYlist(const vector<G4double>& x, const vector<G4double>& y)"
        );
    }
}

XYlist::XYlist(const G4String& csv_file_name, const size_t& data_size) {
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
        ThrowDataFileNotFound(
            "Data file:" + csv_file_name + "not found.\nThis XYlist will stay empty.",
            "XYlist::XYlist(const G4String& csv_file_name, const size_t& data_size)"
        );
    }
}

void XYlist::SetXY(const vector<G4double>& x, const vector<G4double>& y) {
    if (x.size() == y.size()) {
        xylist_.first = x;
        xylist_.second = y;
    } else {
        ThrowLengthMismatches(
            "New x, y are not the same length.\nThis XYlist will stay unchanged.",
            "XYlist::SetXY(const vector<G4double>& x, const vector<G4double>& y)"
        );
    }
}

void XYlist::SetX(const vector<G4double>& x) {
    if (x.size() == xylist_.second.size()) {
        xylist_.first = x;
    } else {
        ThrowLengthMismatches(
            "The new x is not the same length as current y.\nThis XYlist will stay unchanged.",
            "XYlist::SetX(const vector<G4double>& x)"
        );
    }
}

void XYlist::SetY(const vector<G4double>& y) {
    if (y.size() == xylist_.first.size()) {
        xylist_.second = y;
    } else {
        ThrowLengthMismatches(
            "The new y is not the same length as current x.\nThis XYlist will stay unchanged.",
            "XYlist::SetY(const vector<G4double>& y)"
        );
    }
}

void XYlist::ThrowDataFileNotFound(const G4String& error_messages, const G4String& function_name) const {
    G4ExceptionDescription exception_description;
    exception_description << error_messages << G4endl;
    G4Exception(function_name, "DataFileNotFound", JustWarning, exception_description);
}

void XYlist::ThrowLengthMismatches(const G4String& error_messages, const G4String& function_name) const {
    G4ExceptionDescription exception_description;
    exception_description << error_messages << G4endl;
    G4Exception(function_name, "LengthMismatches", JustWarning, exception_description);
}

