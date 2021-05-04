#include "G4Run.hh"

#include "SBProgressMonitor.hh"

SBProgressMonitor* SBProgressMonitor::Instance() {
    static SBProgressMonitor instance;
    return &instance;
}

SBProgressMonitor::SBProgressMonitor() :
    fRunManager(G4MTRunManager::GetMasterRunManager()),
    fRunStartTime(0),
    fRunEndTime(0),
    fTimerStarted(false),
    fTotalEventsInThisRun(0),
    fTotalEvents(0),
    fProcessedEventsInThisRun(0),
    fPreviousProcessedEvents(0),
    fEventsPerReport(1),
    fCPUTime(0) {}

void SBProgressMonitor::SetNumberOfEventsPerReport(G4int n) {
    fEventsPerReport = n;
}

void SBProgressMonitor::SetTotalEvents(G4int n) {
    if (fTimerStarted) { return; }
    fTotalEvents = n;
}

void SBProgressMonitor::SetPreviousProcessedEvents(G4int n) {
    if (fTimerStarted) { return; }
    fPreviousProcessedEvents = n;
}

void SBProgressMonitor::RunStart() {
    if (fTimerStarted) { return; }
    time(&fRunStartTime);
    fTotalEventsInThisRun = fRunManager->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    if (fTotalEvents < fTotalEventsInThisRun) { fTotalEvents = fTotalEventsInThisRun; }
    fProcessedEventsInThisRun = 0;
    fCPUTime = clock();
    fTimerStarted = true;
}

G4Mutex mutex_SBProgressMonitor;

void SBProgressMonitor::EventComplete() {
    if (!fTimerStarted) { return; }
    mutex_SBProgressMonitor.lock();
    ++fProcessedEventsInThisRun;
    if ((fPreviousProcessedEvents + fProcessedEventsInThisRun) % fEventsPerReport == 0) {
        time_t currentTime = time(nullptr);
        G4int estTimeRemain = (fTotalEventsInThisRun - fProcessedEventsInThisRun)
            * float(currentTime - fRunStartTime) / fProcessedEventsInThisRun;
        G4int estTimeRemainHr = estTimeRemain / 3600;
        G4int estTimeRemainMin = (estTimeRemain - estTimeRemainHr * 3600) / 60;
        G4int estTimeRemainSec = estTimeRemain - (estTimeRemainMin * 60 + estTimeRemainHr * 3600);
        auto totalProcessedEvents = fPreviousProcessedEvents + fProcessedEventsInThisRun;
        std::cout
            << ctime(&currentTime)
            << totalProcessedEvents << '/' << fTotalEvents << " events processed ("
            << float(100 * totalProcessedEvents) / fTotalEvents << "%). "
            << "ETA: " << estTimeRemainHr << "h " << estTimeRemainMin << "m " << estTimeRemainSec << 's'
            << std::endl;
    }
    mutex_SBProgressMonitor.unlock();
}

void SBProgressMonitor::RunComplete() {
    if (!fTimerStarted) { return; }
    auto run = G4MTRunManager::GetMasterRunManager()->GetCurrentRun();
    if (run->GetNumberOfEvent() == run->GetNumberOfEventToBeProcessed()) {
        time(&fRunEndTime);
        fCPUTime = clock() - fCPUTime;
        fTimerStarted = false;
        std::cout
            << '\n'
            << "  Start time: " << ctime(&fRunStartTime)
            << "    End time: " << ctime(&fRunEndTime)
            << "Elapsed time: " << difftime(fRunEndTime, fRunStartTime) << "s\n"
            << "    CPU time: " << fCPUTime / CLOCKS_PER_SEC << 's'
            << std::endl;
    }
}

