from ROOT import TCanvas, TFile, TPad
from ROOT import TH1F, TH1I, TH2F
from ROOT import gROOT, gStyle

tfile = TFile.Open("sim.root", "read")

sim_data = tfile.Get("simData")
all_data = tfile.Get("allData")

''' simData Branches
gunEnergy
gridEnergy [vector]
gridID [vector]
gridTrackID [vector]
scintXPosition [vector]
scintYPosition [vector]
scintZPosition [vector]
scintEnergy [vector]
scintTime [vector]
scintTrackID [vector]
scintMass [vector]
scintCharge [vector]
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
'''

''' Reaction Types
0: 4He, 6/7Li or 7Be with trackID = 1 there is no reaction
1: 8B with trackID = 3 there was (d, n)
2: 7Be or 1H with trackID = 5, 4 there was (d, n)8B*
'''

# Output Files
output_root = TFile("output.root", "recreate")

# Canvas Definitions
#c1 = TCanvas('c1', 'Scint. Energy vs Grid Energy', 200, 10, 800, 600 )
c2 = TCanvas('scintGridRT', 'Scint. Energy vs Grid Energy with Reaction Types', 500, 10, 800, 600 )

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

h_scint_grid_mp = []
h_scint_grid_mp.append(TH2F('scintGridMP0', 'Scint. Energy vs Grid Energy with Multiple Particles; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
h_scint_grid_mp.append(TH2F('scintGridMP1', 'Scint. Energy vs Grid Energy with Multiple Particles; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))

h_multiple_particles = TH2F('multiple_particles', 'Mass vs Charge of Events with > 1 Particles; Mass; Charge', 9, 0, 9, 9, 0, 9)

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

  # Get events with multiple particles hitting scintillator
  multiple_particles = 0
  for i in range(len(sim_data.scintTrackID)):
    if i == 0:
      previousTrack = sim_data.scintTrackID[i]
    else:
      currentTrack = sim_data.scintTrackID[i]
      if(previousTrack != currentTrack):
        multiple_particles = 1

  if multiple_particles:
    for i in range(len(sim_data.scintTrackID)):
      h_multiple_particles.Fill(sim_data.scintMass[i], sim_data.scintCharge[i])

  # Get energy in scintillator
  scintillator_energy = 0
  for energy in sim_data.scintEnergy:
    scintillator_energy += energy

  # Get emergy in grids
  grid_energy = 0
  for energy in sim_data.gridEnergy:
    grid_energy += energy

  # Fill Histograms
  h_scint_grid.Fill(scintillator_energy, grid_energy)
  h_scint_grid_rt[reaction_type].Fill(scintillator_energy, grid_energy)
  h_scint_grid_mp[multiple_particles].Fill(scintillator_energy, grid_energy)

############################
# Done with event by event #
############################

# Write histograms to file
h_scint_grid.Write()
for i in range(len(h_scint_grid_rt)):
  h_scint_grid_rt[i].Write()
for i in range(len(h_scint_grid_mp)):
  h_scint_grid_mp[i].Write()
h_multiple_particles.Write()

# Draw histograms
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
c2.Write()

input('Press Enter to Exit')