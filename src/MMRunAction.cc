#include "MMRunAction.hh"

MMRunAction::MMRunAction(MMDetectorConstruction* detector, MMPrimaryGeneratorAction* primary) :
  G4UserRunAction(), fDetector(detector), fPrimary(primary) {
  MMAnalysis::Instance();
}

MMRunAction::~MMRunAction() {
  delete MMAnalysis::Instance();
}

void MMRunAction::BeginOfRunAction(const G4Run*) {
  if(G4Threading::G4GetThreadId() < 0) return;

  G4cout << "Thread ID is: " << G4Threading::G4GetThreadId() << G4endl;

  bool printTable = true;
  if(printTable) {
    G4Material* material = fDetector->GetGasMaterial();
    G4ProductionCutsTable* theCoupleTable = G4ProductionCutsTable::GetProductionCutsTable();
    size_t numOfCouples = theCoupleTable->GetTableSize();
    const G4MaterialCutsCouple* couple = 0;
    G4int index = 0;
    for(size_t i = 0; i < numOfCouples; i++) {
      couple = theCoupleTable->GetMaterialCutsCouple(i);
      if(couple->GetMaterial() == material) {
        index = i;
        break;
      }
    }

    // G4double energyCut[3];
    // energyCut[0] = (*(theCoupleTable->GetEnergyCutsVector(idxG4GammaCut)))[index];
    // energyCut[1] = (*(theCoupleTable->GetEnergyCutsVector(idxG4ElectronCut)))[index];
    // energyCut[2] = (*(theCoupleTable->GetEnergyCutsVector(idxG4PositronCut)))[index];

    // G4ParticleDefinition* particle = fPrimary->GetParticleGun()->GetParticleDefinition();
    // G4ProcessVector* plist = particle->GetProcessManager()->GetProcessList();

    // std::vector<G4String> emName;
    // std::vector<G4double> enerCut;
    // size_t length = plist->size();
    // for(size_t j = 0; j < length; j++) {
    //   G4String procName = (*plist)[j]->GetProcessName();
    //   G4double cut = energyCut[1];
    //   if((procName == "eBrem") || (procName == "muBrems")) cut = energyCut[0];
    //   if(((*plist)[j]->GetProcessType() == fElectromagnetic) && (procName != "msc")) {
    //     emName.push_back(procName);
    //     enerCut.push_back(cut);
    //   }
    // }
  }
}

void MMRunAction::EndOfRunAction(const G4Run*) {
  MMAnalysis::Instance()->CloseFile();
}
