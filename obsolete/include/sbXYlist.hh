#ifndef SB_XYLIST_HH
#define SB_XYLIST_HH 1

#include <utility>
#include <vector>
#include <fstream>

using std::vector;

#include "globals.hh"

class XYlist {
private:
    std::pair<vector<G4double>, vector<G4double>> xylist_;

public:
    XYlist(const vector<G4double>& x, const vector<G4double>& y);
    XYlist(const G4String& csv_file_name, const size_t& data_size);
    ~XYlist() {}
    G4bool IsEmpty() const { return xylist_.first.empty(); }
    size_t Size() const { return xylist_.first.size(); }
    vector<G4double>& x() { return xylist_.first; }
    vector<G4double>& y() { return xylist_.second; }
    const vector<G4double>& cx() const { return xylist_.first; }
    const vector<G4double>& cy() const { return xylist_.second; }
    G4double* px() { return &*(xylist_.first.begin()); }
    G4double* py() { return &*(xylist_.second.begin()); }
    const G4double* cpx() const { return &*(xylist_.first.begin()); }
    const G4double* cpy() const { return &*(xylist_.second.begin()); }
    void SwapXY() { xylist_.first.swap(xylist_.second); }
    void SetXY(const vector<G4double>& x, const vector<G4double>& y);
    void SetX(const vector<G4double>& x);
    void SetY(const vector<G4double>& y);

private:
    void ThrowDataFileNotFound(const G4String& error_messages, const G4String& function_name) const;
    void ThrowLengthMismatches(const G4String& error_messages, const G4String& function_name) const;
};

#endif

