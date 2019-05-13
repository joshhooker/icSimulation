import numpy as np
from sklearn.metrics import silhouette_score
import sys

from ROOT import TCanvas, TFile, TLegend, TPad
from ROOT import TH1F, TH1I, TH2F
from ROOT import gROOT, gStyle

def analysis(input_file, output_file):
  tfile = TFile.Open(input_file, "read")

  sim_data = tfile.Get("simData")
  all_data = tfile.Get("allData")

  ''' simData Branches
  gridEnergy [vector<double>]
  gridID [vector<int>]
  gridTrackID [vector<int>]
  scintXPosition [vector<double>]
  scintYPosition [vector<double>]
  scintZPosition [vector<double>]
  scintEnergy [vector<double>]
  scintTrackID [vector<int>]
  scintMass [vector<int>]
  scintCharge [vector<int>]
  beamEnergy [double]
  beamCharge [int]
  beamMass [int]
  energy [double]
  cmEnergy [double]
  vertex [double]
  lightAngleCM [double]
  lightAngleLab [double]
  lightEnergy [double]
  heavyAngleCM [double]
  heavyAngleLab [double]
  heavyEnergy [double]
  qValue [double]
  excitedEnergy [double]
  lightRecoilCharge [int]
  lightRecoilMass [int]
  heavyRecoilCharge [int]
  heavyRecoilMass [int]
  '''

  ''' Reaction Types
  0: 4He, 6/7Li or 7Be with trackID = 1 there is no reaction
  1: 8B with trackID = 3 there was (d, n)
  2: 7Be or 1H with trackID = 5, 4 there was (d, n)8B*
  '''

  ''' Scattering Types
  0: No scattering
  1: (d, d)
  2: (12C, 12C)
  3: (d, n)
  '''

  # Output Files
  output_root = TFile(output_file, "recreate")

  # Canvas Definitions
  #c1 = TCanvas('c1', 'Scint. Energy vs Grid Energy', 200, 10, 800, 600 )
  c2 = TCanvas('scintGridRT', 'Scint. Energy vs Grid Energy with Reaction Types', 500, 10, 800, 600 )
  c3 = TCanvas('scintGridST', 'Scint. Energy vs Grid Energy with Scattering Types', 500, 10, 800, 600 )

  gStyle.SetOptStat(10)
  gStyle.SetStatW(0.1)
  gStyle.SetStatH(0.1)

  # Histogram Definitions
  h_scint_grid = TH2F('scintGrid', 'Scint. Energy vs Grid Energy; Scintillator Energy [MeV]; Total Grid Energy [MeV]',
    500, 0, 40, 500, 0, 4)
  h_scint_grid.GetXaxis().CenterTitle(); h_scint_grid.GetYaxis().CenterTitle()

  h_scint_grid_rt = []
  h_scint_grid_rt.append(TH2F('scintGridRT0', 'Scint. Energy vs Grid Energy with Reaction Types; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
  h_scint_grid_rt.append(TH2F('scintGridRT1', 'Scint. Energy vs Grid Energy with Reaction Types; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
  h_scint_grid_rt.append(TH2F('scintGridRT2', 'Scint. Energy vs Grid Energy with Reaction Types; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
  for i in range(len(h_scint_grid_rt)):
    h_scint_grid_rt[i].GetXaxis().CenterTitle(); h_scint_grid_rt[i].GetYaxis().CenterTitle()

  h_scint_grid_st = []
  h_scint_grid_st.append(TH2F('scintGridST0', 'Scint. Energy vs Grid Energy with Scattering Types; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
  h_scint_grid_st.append(TH2F('scintGridST1', 'Scint. Energy vs Grid Energy with Scattering Types; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
  h_scint_grid_st.append(TH2F('scintGridST2', 'Scint. Energy vs Grid Energy with Scattering Types; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
  h_scint_grid_st.append(TH2F('scintGridST3', 'Scint. Energy vs Grid Energy with Scattering Types; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
  for i in range(len(h_scint_grid_st)):
    h_scint_grid_st[i].GetXaxis().CenterTitle(); h_scint_grid_st[i].GetYaxis().CenterTitle()

  h_scint_grid_mp = []
  h_scint_grid_mp.append(TH2F('scintGridMP0', 'Scint. Energy vs Grid Energy with Multiple Particles; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
  h_scint_grid_mp.append(TH2F('scintGridMP1', 'Scint. Energy vs Grid Energy with Multiple Particles; Scintillator Energy [MeV]; Total Grid Energy [MeV]', 500, 0, 40, 500, 0, 4))
  for i in range(len(h_scint_grid_mp)):
    h_scint_grid_mp[i].GetXaxis().CenterTitle(); h_scint_grid_mp[i].GetYaxis().CenterTitle()

  h_multiple_particles = TH2F('multiple_particles', 'Mass vs Charge of Events with > 1 Particles; Mass; Charge', 9, 0, 9, 9, 0, 9)
  h_multiple_particles.GetXaxis().CenterTitle(); h_multiple_particles.GetYaxis().CenterTitle()

  h_scint_position = TH2F('scint_pos', 'X-Y Position of Hits in the Scintillator; X [mm]; Y [mm]', 2000, -100, 100, 2000, -100, 100)
  h_scint_position.GetXaxis().CenterTitle(); h_scint_position.GetYaxis().CenterTitle()

  h_vertex_position = TH1F('vertex_pos', 'Vertex Position of Reaction; Z[mm]', 200, -20, 200)
  h_vertex_position.GetXaxis().CenterTitle(); h_vertex_position.GetYaxis().CenterTitle()

  # Define variables for silhouette score
  silhouette_arr = []
  silhouette_label_2_clusters = []
  silhouette_label_3_clusters = []

  # Define variables for efficiency
  observed = [0]*3
  generated = [0]*3

  # Loop over all events in simData
  for event in sim_data:

    # Get Reaction Type
    reaction_type = 0
    if(event.beamCharge == 4 and event.lightRecoilCharge == 0 and event.lightRecoilMass == 1):
      if(event.excitedEnergy < 0.001):
        reaction_type = 1
      else:
        reaction_type = 2

    # Get Scattering Type
    scattering_type = 0
    if event.lightRecoilCharge == 1 and event.lightRecoilMass == 2:
      scattering_type = 1
    elif event.heavyRecoilCharge == 6 and event.heavyRecoilMass == 12:
      scattering_type = 2
    elif event.lightRecoilCharge == 0 and event.lightRecoilMass == 1:
      scattering_type = 3

    generated[reaction_type] += 1

    # Get events with multiple particles hitting scintillator
    multiple_particles = 0
    for i in range(len(event.scintTrackID)):
      if i == 0:
        previousTrack = event.scintTrackID[i]
      else:
        currentTrack = event.scintTrackID[i]
        if(previousTrack != currentTrack):
          multiple_particles = 1

    if multiple_particles:
      for i in range(len(event.scintTrackID)):
        h_multiple_particles.Fill(event.scintMass[i], event.scintCharge[i])

    # Get energy in scintillator
    scintillator_energy = np.sum(event.scintEnergy)

    # Get emergy in grids
    grid_energy = np.sum(event.gridEnergy)

    tmp_arr = [scintillator_energy, grid_energy]
    silhouette_arr.append(tmp_arr)
    silhouette_label_2_clusters.append(1 if reaction_type > 0 else 0)
    silhouette_label_3_clusters.append(reaction_type)

    # Fill Histograms
    if scintillator_energy > 0.001:
      h_scint_grid.Fill(scintillator_energy, grid_energy)
      h_scint_grid_rt[reaction_type].Fill(scintillator_energy, grid_energy)
      h_scint_grid_st[scattering_type].Fill(scintillator_energy, grid_energy)
      h_scint_grid_mp[multiple_particles].Fill(scintillator_energy, grid_energy)

      scintAvgXPos = np.mean(event.scintXPosition)
      scintAvgYPos = np.mean(event.scintYPosition)
      h_scint_position.Fill(scintAvgXPos, scintAvgYPos)

      observed[reaction_type] += 1

    h_vertex_position.Fill(event.vertex)

  ############################
  # Done with event by event #
  ############################

  # Write histograms to file
  h_scint_grid.Write()
  for i in range(len(h_scint_grid_rt)):
    h_scint_grid_rt[i].Write()
  for i in range(len(h_scint_grid_st)):
    h_scint_grid_st[i].Write()
  for i in range(len(h_scint_grid_mp)):
    h_scint_grid_mp[i].Write()
  h_multiple_particles.Write()
  h_scint_position.Write()
  h_vertex_position.Write()

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
  legend_rt = TLegend(0.6, 0.75, 0.9, 0.9)
  legend_rt.AddEntry(h_scint_grid_rt[0], "No Reaction/Other", "p")
  legend_rt.AddEntry(h_scint_grid_rt[1], "(d, n)^{8}B", "p")
  legend_rt.AddEntry(h_scint_grid_rt[2], "(d, n)^{8}B*", "p")
  legend_rt.Draw()
  c2.Update()
  c2.Write()

  # Draw Scintillator Energy vs Grid Energy for Scattering Types
  c3.cd()
  h_scint_grid_st[1].SetMarkerStyle(20)
  h_scint_grid_st[1].SetMarkerSize(0.3)
  h_scint_grid_st[1].SetMarkerColor(1)
  h_scint_grid_st[1].Draw("P")
  h_scint_grid_st[2].SetMarkerStyle(20)
  h_scint_grid_st[2].SetMarkerSize(0.3)
  h_scint_grid_st[2].SetMarkerColor(2)
  h_scint_grid_st[2].Draw("PSame")
  h_scint_grid_st[3].SetMarkerStyle(20)
  h_scint_grid_st[3].SetMarkerSize(0.3)
  h_scint_grid_st[3].SetMarkerColor(3)
  h_scint_grid_st[3].Draw("PSame")
  legend_st = TLegend(0.6, 0.75, 0.9, 0.9)
  legend_st.AddEntry(h_scint_grid_st[1], "(d, d)", "p")
  legend_st.AddEntry(h_scint_grid_st[2], "(^{12}C, ^{12}C)", "p")
  legend_st.AddEntry(h_scint_grid_st[3], "(d, n)", "p")
  legend_st.Draw()
  c3.Update()
  c3.Write()

  # silhouette_avg_2_clusters = silhouette_score(silhouette_arr, silhouette_label_2_clusters)
  # print("Silhouette Score for 2 Clusters: ", silhouette_avg_2_clusters)

  # silhouette_avg_3_clusters = silhouette_score(silhouette_arr, silhouette_label_3_clusters)
  # print("Silhouette Score for 3 Clusters: ", silhouette_avg_3_clusters)

  print("Observed/Generated Ratios:")
  for i in range(len(observed)):
    print("Reaction Type %d: %f", i, observed[i]/generated[i])

  input('Press Enter to Exit')

if __name__ == "__main__":
  if(len(sys.argv) != 3):
    print("To run: python analysis.py input_filename output_filename")
  else:
    analysis(sys.argv[1], sys.argv[2])