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
    XYlist(const char csv_file_name[], const size_t& data_size);
    ~XYlist() {}
    G4bool IsEmpty() const { return xylist_.first.empty(); }
    size_t Size() const { return xylist_.first.size(); }
    const vector<G4double>& x() const { return xylist_.first; }
    const vector<G4double>& y() const { return xylist_.second; }
    void SwapXY() { xylist_.first.swap(xylist_.second); }
    void SetXY(const vector<G4double> x, const vector<G4double> y);
    void SetX(const vector<G4double> x);
    void SetY(const vector<G4double> y);
};

#endif

