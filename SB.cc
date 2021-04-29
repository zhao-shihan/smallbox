#include <set>

#include "G4GlobalConfig.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "SBDetectorConstruction.hh"
#include "SBActionInitialization.hh"
#include "SBPhysicsList.hh"
#include "SBAnalysisManager.hh"
#include "SBPrimaryGeneratorAction.hh"
#include "SBConfigs.hh"
#include "SBProgressMonitor.hh"

int main(int argc, char** argv) {
    if (argc < 1 || argc > 11) {
        std::cout <<
            "Usage: SB (options)\n"
            "Options:\n"
            "\t-event [EventListFileName]\n"
            "\t-thread [NumberOfThreads]\n"
            "\t-name [NameOfRun]\n"
            "\t-seed [RandomSeed]\n"
            "\t-vis\n"
            "\t-rerun" << std::endl;
        return 1;
    }

    auto maxLogicalCores = std::thread::hardware_concurrency();

    std::string eventListFileName("");
    size_t numberOfThreads = maxLogicalCores / 2;
    std::string name("unnamed");
    bool isBatch = true;
    bool reRun = false;
    for (int i = 1; i < argc; ++i) {
        std::string argvStr(argv[i]);
        if (argvStr == "-event") {
            eventListFileName = argv[i + 1];
        } else if (argvStr == "-thread") {
            numberOfThreads = std::stoll(argv[i + 1]);
            if (numberOfThreads > maxLogicalCores) {
                std::cout << "Warning: The number of threads (" << numberOfThreads << ") "
                    "is greater than the number of logical cores available (" << maxLogicalCores << ")." << std::endl;
            } else if (numberOfThreads > maxLogicalCores / 2) {
                std::cout << "Warning: The number of threads (" << numberOfThreads << ") "
                    "is greater than the number of physical cores available (" << maxLogicalCores / 2 << ")."
                    " (Assuming hyper-threading.)" << std::endl;
            }
        } else if (argvStr == "-name") {
            name = argv[i + 1];
        } else if (argvStr == "-seed") {
            G4Random::setTheSeed(std::stol(argv[i + 1]));
        } else if (argvStr == "-vis") {
            isBatch = false;
        } else if (argvStr == "-rerun") {
            reRun = true;
        }
    }

    if (isBatch) {
#if !SB_SAVE_ANYTHING
        std::cerr << "There is nothing to save! "
            "If you want to run in batch mode, try saving something." << std::endl;
        return 1;
#endif
        if (eventListFileName.empty()) {
            std::cerr << "SB has nothing to do!\n"
                "If you want to run in batch mode, please define a parameter grid,\n"
                "or use \"-vis\" flag to run in interactive mode." << std::endl;
            return 1;
        }
    } else {
        name.append("_vis");
    }

    SBAnalysisManager::SetFileName(name);

    G4MTRunManager* runManager = new G4MTRunManager();
    runManager->SetNumberOfThreads(numberOfThreads);
    runManager->SetUserInitialization(new SBDetectorConstruction());
    runManager->SetUserInitialization(new SBPhysicsList());
    runManager->SetUserInitialization(new SBActionInitialization());

    if (!eventListFileName.empty()) {
        auto&& eventList = CreateMapFromCSV<std::string, double>(eventListFileName);
        std::cout << "Reading grid <" << eventListFileName << ">..." << std::flush;
        if (eventList.empty()) {
            std::cerr << "Error: Failed to create event list, execution terminated." << std::endl;
            return 1;
        }
        std::cout << " done." << std::endl;
        
        if (eventList.find("propertyGrid") != eventList.end()) {
#if SB_SAVE_TYPE == 1
            std::list<SBMuonProperty> existedEventList;
            size_t numberOfExisted = 0;
            if (!reRun) {
                std::ifstream fin(name + ".csv");
                if (fin.is_open()) {
                    std::cout << "File <" << name << ".csv> found, looking for existing events..." << std::endl;
                    std::string line;
                    std::string element;
                    if (std::getline(fin, line)) { SBAnalysisManager::IsContinue(); }
                    while (std::getline(fin, line)) {
                        existedEventList.push_back(SBMuonProperty());
                        std::istringstream lineStream(line);
                        std::getline(lineStream, element, ',');
                        existedEventList.back().X() = std::stod(element) * cm;
                        std::getline(lineStream, element, ',');
                        existedEventList.back().Y() = std::stod(element) * cm;
                        std::getline(lineStream, element, ',');
                        existedEventList.back().Phi() = std::stod(element) * rad;
                        std::getline(lineStream, element, ',');
                        existedEventList.back().Theta() = std::stod(element) * rad;
                        std::getline(lineStream, element, ',');
                        existedEventList.back().Energy() = std::stod(element) * MeV;
                    }
                    numberOfExisted = existedEventList.size();
                    std::cout << "Done. " << existedEventList.size() << " events found." << std::endl;
                    fin.close();
                } else {
                    std::cout << "Warning: Cannot open <" << name << ".csv>.\n"
                        "It's OK if this is the first run of the grid.\n"
                        "If not, it means that the previous results cannot be read or have been deleted,\n"
                        "the simulation cannot be continued and must be restarted." << std::endl;
                }
            }
#endif
            size_t numberOfRepeat = eventList["propertyGrid"].front();
            std::set<double> xSet, ySet, phiSet, thetaSet, energySet;
            for (const auto& x : eventList["x"]) { xSet.insert(x); }
            for (const auto& y : eventList["y"]) { ySet.insert(y); }
            for (const auto& phi : eventList["phi"]) { phiSet.insert(phi); }
            for (const auto& theta : eventList["theta"]) { thetaSet.insert(theta); }
            for (const auto& energy : eventList["energy"]) { energySet.insert(energy); }
            size_t numberOfEventInOneRepetition =
                energySet.size() * xSet.size() * ySet.size() * phiSet.size() * thetaSet.size();
            size_t numberOfEvent = numberOfEventInOneRepetition * numberOfRepeat;
            std::cout << "The grid consists of " << numberOfEventInOneRepetition << " events, repeated by "
                << numberOfRepeat << ", there are " << numberOfEvent << " events in total." << std::endl;

            std::cout << "Creating event list for this run, excluding the existing events..." << std::endl;
            std::vector<SBMuonProperty> muonEventList;
            muonEventList.reserve(numberOfEvent);
            for (size_t repeatCount = 0; repeatCount < numberOfRepeat; ++repeatCount) {
                for (const auto& x : xSet) {
                    for (const auto& y : ySet) {
                        for (const auto& phi : phiSet) {
                            for (const auto& theta : thetaSet) {
                                for (const auto& energy : energySet) {
                                    SBMuonProperty event(x * cm, y * cm, phi * rad, theta * rad, energy * MeV);
                                    auto existedEvent =
                                        std::find(existedEventList.begin(), existedEventList.end(), event);
                                    if (existedEvent == existedEventList.end()) {
                                        muonEventList.push_back(event);
                                    } else {
                                        existedEventList.erase(existedEvent);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            muonEventList.shrink_to_fit();
            SBPrimaryGeneratorAction::SetMuonEventList(muonEventList);
            std::cout << "Done. This run consists of " << muonEventList.size() << " events." << std::endl;

            SBProgressMonitor::Instance()->SetPreviousProcessedEvents(numberOfExisted);
            SBProgressMonitor::Instance()->SetTotalEvents(numberOfEvent);
            std::cout <<
                "\n[Pre run info]\n"
                "\tGrid consists: " << numberOfEventInOneRepetition << "\n"
                "\t       Repeat: " << numberOfRepeat << "\n"
                "\t        Total: " << numberOfEvent << "\n"
                "\tAlready exist: " << numberOfExisted << "\n"
                "\t    Remaining: " << muonEventList.size() << "\n" << std::endl;
            if (isBatch) {
                runManager->Initialize();
                runManager->BeamOn(muonEventList.size());
            } else {
                G4UIExecutive* ui = new G4UIExecutive(argc, argv);
                G4VisManager* visManager = new G4VisExecutive();
                G4UImanager* UImanager = G4UImanager::GetUIpointer();

                visManager->Initialize();
                UImanager->ApplyCommand("/control/macroPath macros");
                UImanager->ApplyCommand("/control/execute init_vis.mac");
                runManager->BeamOn(muonEventList.size());
                ui->SessionStart();

                delete ui;
                delete visManager;
            }
        } else if (eventList.find("cosmicMuonEvents") != eventList.end()) {
            if (isBatch) {
                runManager->Initialize();
                runManager->BeamOn(eventList["cosmicMuonEvents"].front());
            } else {
                G4UIExecutive* ui = new G4UIExecutive(argc, argv);
                G4VisManager* visManager = new G4VisExecutive();
                G4UImanager* UImanager = G4UImanager::GetUIpointer();

                visManager->Initialize();
                UImanager->ApplyCommand("/control/macroPath macros");
                UImanager->ApplyCommand("/control/execute init_vis.mac");
                runManager->BeamOn(eventList["cosmicMuonEvents"].front());
                ui->SessionStart();

                delete ui;
                delete visManager;
            }
        }
    } else {
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        G4VisManager* visManager = new G4VisExecutive();
        G4UImanager* UImanager = G4UImanager::GetUIpointer();

        visManager->Initialize();
        UImanager->ApplyCommand("/control/macroPath macros");
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();

        delete ui;
        delete visManager;
    }

    delete runManager;

    return 0;
}

