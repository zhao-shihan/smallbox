#ifndef SB_PROGRESS_MANAGER
#define SB_PROGRESS_MANAGER 1

#include <ctime>
#include "globals.hh"
#include "G4MTRunManager.hh"

// Class for computing time management and estimation.
//
class SBProgressMonitor {
public:
    static SBProgressMonitor* Instance();
private:
    SBProgressMonitor();
    ~SBProgressMonitor() {}
    SBProgressMonitor(const SBProgressMonitor&) = delete;
    SBProgressMonitor& operator=(const SBProgressMonitor&) = delete;

private:
    const G4MTRunManager* fRunManager;

    time_t  fRunStartTime;
    time_t  fRunEndTime;
    G4bool  fTimerStarted;
    G4int   fTotalEventsInThisRun;
    G4int   fTotalEvents;
    G4int   fProcessedEventsInThisRun;
    G4int   fPreviousProcessedEvents;
    G4int   fEventsPerReport;
    clock_t fCPUTime;

public:
    void SetNumberOfEventsPerReport(G4int n);
    void SetTotalEvents(G4int n);
    void SetPreviousProcessedEvents(G4int n);

    void RunStart();
    void EventComplete();
    void RunComplete();
};

#endif

