from ROOT import TCanvas, TFile, TPad
from ROOT import TH1F, TH2F
from ROOT import gROOT, gStyle

tfile = TFile.Open("sim.root", "read")

sim_data = tfile.Get("simData")
all_data = tfile.Get("allData")

''' simData Branches
gunEnergy
icGridEnergy [vector]
icGridTotalEnergy
scintXPos [vector]
scintYPos [vector]
scintZPos [vector]
scintE [vector]
scintT [vector]
scintTrackID [vector]
scintMass [vector]
scintCharge [vector]
reactionType
'''

''' allData Branches
energy
cmEnergy
lighAngleCM
lightAngleLab
lightEnergy
heavyAngleCM
heavyAngleLab
heavyEnergy
reactionType
'''

''' Reaction Types
0: 4He, 6/7Li or 7Be with trackID = 1 there is no reaction
1: 8B with trackID = 3 there was (d, n)
2: 7Be or 1H with trackID = 5, 4 there was (d, n)8B*
'''

c1 = TCanvas('c1', 'Scint. Energy vs Grid Energy', 200, 10, 800, 600 )
c2 = TCanvas('c2', 'Scint. Energy vs Grid Energy with Reaction Types', 500, 10, 800, 600 )

gStyle.SetOptStat(10)
gStyle.SetStatW(0.1)
gStyle.SetStatH(0.1)

# Histogram Definitions
h_scint_grid = TH2F('scintGrid', 'Scint. Energy vs Grid Energy; Scintillator Energy [MeV]; Total Grid Energy [MeV]',
  500, 0, 40, 500, 0, 4)

h_scint_grid_rt = []
h_scint_grid_rt.append(TH2F('scintGridRT0', 'Scint. Energy vs Grid Energy with Reaction Types; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
h_scint_grid_rt.append(TH2F('scintGridRT1', 'Scint. Energy vs Grid Energy with Reaction Types; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
h_scint_grid_rt.append(TH2F('scintGridRT2', 'Scint. Energy vs Grid Energy with Reaction Types; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))


# Loop over all events in simData
for event in sim_data:

  # Get reactionType
  reaction_type = 0
  for i in range(len(sim_data.scintTrackID)):
    if(sim_data.scintTrackID[i] == 1):
      reaction_type = 0
    elif(sim_data.scintTrackID[i] == 3 and sim_data.scintCharge[i] == 5 and sim_data.scintMass[i] == 8):
      reaction_type = 1
    elif((sim_data.scintTrackID[i] == 5 and sim_data.scintCharge[i] == 4 and sim_data.scintMass[i] == 7) or
      (sim_data.scintTrackID[i] == 4 and sim_data.scintCharge[i] == 1 and sim_data.scintMass[i] == 1)):
      reaction_type = 2


  # Get energy in scintillator
  scintillator_energy = 0
  for energy in sim_data.scintE:
    scintillator_energy += energy

  # Get emergy in grids
  grid_energy = sim_data.icGridTotalEnergy


  # Fill Histograms
  h_scint_grid.Fill(scintillator_energy, grid_energy)
  h_scint_grid_rt[reaction_type].Fill(scintillator_energy, grid_energy)



c1.cd()
h_scint_grid.Draw()
c1.Update()

c2.cd()
h_scint_grid_rt[0].SetMarkerStyle(20)
h_scint_grid_rt[0].SetMarkerSize(0.3)
h_scint_grid_rt[0].SetMarkerColor(1)
h_scint_grid_rt[0].Draw("P")
h_scint_grid_rt[1].SetMarkerStyle(20)
h_scint_grid_rt[1].SetMarkerSize(0.3)
h_scint_grid_rt[1].SetMarkerColor(2)
h_scint_grid_rt[1].Draw("PSame")
h_scint_grid_rt[2].SetMarkerStyle(20)
h_scint_grid_rt[2].SetMarkerSize(0.3)
h_scint_grid_rt[2].SetMarkerColor(3)
h_scint_grid_rt[2].Draw("PSame")
c2.Update()

input('Press Enter to Exit')