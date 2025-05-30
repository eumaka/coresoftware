// This is the new trackbase container version

#include "PHG4InttDigitizer.h"

#include <intt/InttMapping.h>

#include <g4detectors/PHG4CylinderGeom.h>
#include <g4detectors/PHG4CylinderGeomContainer.h>

// Move to new storage containers
#include <trackbase/InttDefs.h>
#include <trackbase/TrkrDefs.h>
#include <trackbase/TrkrHit.h>  // for TrkrHit
#include <trackbase/TrkrHitSet.h>
#include <trackbase/TrkrHitSetContainer.h>
#include <trackbase/TrkrHitTruthAssoc.h>

#include <phparameter/PHParameterInterface.h>  // for PHParameterInterface

#include <fun4all/Fun4AllBase.h>  // for Fun4AllBase::VERB...
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/SubsysReco.h>  // for SubsysReco

#include <phool/PHCompositeNode.h>
#include <phool/PHNode.h>  // for PHNode
#include <phool/PHNodeIterator.h>
#include <phool/PHRandomSeed.h>
#include <phool/getClass.h>
#include <phool/phool.h>  // for PHWHERE

#include <TSystem.h>

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>  // for gsl_rng_alloc

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>  // for exit
#include <iostream>
#include <set>
#include <utility> 

PHG4InttDigitizer::PHG4InttDigitizer(const std::string &name)
  : SubsysReco(name)
  , PHParameterInterface(name)
{
  InitializeParameters();
  unsigned int seed = PHRandomSeed();  // fixed seed is handled in this funtcion
  std::cout << Name() << " random seed: " << seed << std::endl;
  RandomGenerator = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(RandomGenerator, seed);
}

PHG4InttDigitizer::~PHG4InttDigitizer()
{
  gsl_rng_free(RandomGenerator);
}

int PHG4InttDigitizer::InitRun(PHCompositeNode *topNode)
{
  std::cout << "PHG4InttDigitizer::InitRun: detector = " << detector << std::endl;

  //-------------
  // Add Hit Node
  //-------------

  PHNodeIterator iter(topNode);

  // Looking for the DST node
  PHCompositeNode *dstNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "DST"));
  if (!dstNode)
  {
    std::cout << PHWHERE << "DST Node missing, doing nothing." << std::endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }

  CalculateLadderCellADCScale(topNode);

  // Create the run and par nodes
  PHCompositeNode *runNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "RUN"));
  PHCompositeNode *parNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "PAR"));

  std::string paramnodename = "G4CELLPARAM_" + detector;
  std::string geonodename = "G4CELLGEO_" + detector;

  UpdateParametersWithMacro();
  // save this to the run wise tree to store on DST
  PHNodeIterator runIter(runNode);
  PHCompositeNode *RunDetNode = dynamic_cast<PHCompositeNode *>(runIter.findFirst("PHCompositeNode", detector));
  if (!RunDetNode)
  {
    RunDetNode = new PHCompositeNode(detector);
    runNode->addNode(RunDetNode);
  }
  SaveToNodeTree(RunDetNode, paramnodename);
  // save this to the parNode for use
  PHNodeIterator parIter(parNode);
  PHCompositeNode *ParDetNode = dynamic_cast<PHCompositeNode *>(parIter.findFirst("PHCompositeNode", detector));
  if (!ParDetNode)
  {
    ParDetNode = new PHCompositeNode(detector);
    parNode->addNode(ParDetNode);
  }
  PutOnParNode(ParDetNode, geonodename);
  mNoiseMean = get_double_param("NoiseMean");
  mNoiseSigma = get_double_param("NoiseSigma");
  mEnergyPerPair = get_double_param("EnergyPerPair");

  /// If user hasn't called with custom calibration, load default
  if (!m_badmap.OfflineLoaded())
  {
    m_badmap.Load(); // Method loads with default tag
  }
  if (Verbosity())
  {
    std::cout << "InttBadChannelMap size: " << m_badmap.size() << std::endl;
  }
  if (VERBOSITY_MORE <= Verbosity())
  {
    m_badmap.Print();
  }

  //----------------
  // Report Settings
  //----------------

  if (Verbosity() > 0)
  {
    std::cout << "====================== PHG4InttDigitizer::InitRun() =====================" << std::endl;
    std::cout << " Masking " << m_badmap.size() << " channels" << std::endl;
    for (auto &iter1 : _max_adc)
    {
      std::cout << " Max ADC in Layer #" << iter1.first << " = " << iter1.second << std::endl;
    }
    for (auto &iter2 : _energy_scale)
    {
      std::cout << " Energy per ADC in Layer #" << iter2.first << " = " << 1.0e6 * iter2.second << " keV" << std::endl;
    }
    std::cout << "===========================================================================" << std::endl;
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

int PHG4InttDigitizer::process_event(PHCompositeNode *topNode)
{
  DigitizeLadderCells(topNode);

  return Fun4AllReturnCodes::EVENT_OK;
}

void PHG4InttDigitizer::CalculateLadderCellADCScale(PHCompositeNode *topNode)
{
  // FPHX 3-bit ADC, thresholds are set in "set_fphx_adc_scale".

  // PHG4CellContainer *cells = findNode::getClass<PHG4CellContainer>(topNode, "G4CELL_INTT");
  PHG4CylinderGeomContainer *geom_container = findNode::getClass<PHG4CylinderGeomContainer>(topNode, "CYLINDERGEOM_INTT");

  // if (!geom_container || !cells) return;
  if (!geom_container)
  {
    return;
  }

  PHG4CylinderGeomContainer::ConstRange layerrange = geom_container->get_begin_end();
  for (PHG4CylinderGeomContainer::ConstIterator layeriter = layerrange.first;
       layeriter != layerrange.second;
       ++layeriter)
  {
    int layer = layeriter->second->get_layer();
    if (_max_fphx_adc.find(layer) == _max_fphx_adc.end())
    {
      std::cout << "Error: _max_fphx_adc is not available." << std::endl;
      gSystem->Exit(1);
    }
    float thickness = (layeriter->second)->get_thickness();  // cm
    float mip_e = 0.003876 * thickness;                      // GeV
    _energy_scale.insert(std::make_pair(layer, mip_e));
  }

  return;
}

void PHG4InttDigitizer::DigitizeLadderCells(PHCompositeNode *topNode)
{
  //---------------------------
  // Get common Nodes
  //---------------------------

  // Get the TrkrHitSetContainer node
  TrkrHitSetContainer *trkrhitsetcontainer = findNode::getClass<TrkrHitSetContainer>(topNode, "TRKR_HITSET");
  if (!trkrhitsetcontainer)
  {
    std::cout << "Could not locate TRKR_HITSET node, quit! " << std::endl;
    exit(1);
  }

  // Get the TrkrHitTruthAssoc node
  auto hittruthassoc = findNode::getClass<TrkrHitTruthAssoc>(topNode, "TRKR_HITTRUTHASSOC");

  //-------------
  // Digitization
  //-------------

  // We want all hitsets for the Intt
  TrkrHitSetContainer::ConstRange hitset_range = trkrhitsetcontainer->getHitSets(TrkrDefs::TrkrId::inttId);
  for (TrkrHitSetContainer::ConstIterator hitset_iter = hitset_range.first;
       hitset_iter != hitset_range.second;
       ++hitset_iter)
  {
    // we have an itrator to one TrkrHitSet for the intt from the trkrHitSetContainer
    // get the hitset key so we can find the layer
    TrkrDefs::hitsetkey hitsetkey = hitset_iter->first;
    const int layer = TrkrDefs::getLayer(hitsetkey);
    const int ladder_phi = InttDefs::getLadderPhiId(hitsetkey);
    const int ladder_z = InttDefs::getLadderZId(hitsetkey);

    if (Verbosity() > 1)
    {
      std::cout << "PHG4InttDigitizer: found hitset with key: " << hitsetkey << " in layer " << layer << std::endl;
    }
    // get all of the hits from this hitset
    TrkrHitSet *hitset = hitset_iter->second;
    TrkrHitSet::ConstRange hit_range = hitset->getHits();
    std::set<TrkrDefs::hitkey> dead_hits;  // hits on dead channel
    for (TrkrHitSet::ConstIterator hit_iter = hit_range.first;
         hit_iter != hit_range.second;
         ++hit_iter)
    {
      ++m_nCells;

      TrkrHit *hit = hit_iter->second;
      TrkrDefs::hitkey hitkey = hit_iter->first;
      int strip_col = InttDefs::getCol(hitkey);  // strip z index
      int strip_row = InttDefs::getRow(hitkey);  // strip phi index

      InttNameSpace::Offline_s ofl;
      ofl.layer = layer;
      ofl.ladder_phi = ladder_phi;
      ofl.ladder_z = ladder_z;
      ofl.strip_x = strip_row;
      ofl.strip_y = strip_col;

      // Apply deadmap here if desired
      if (m_badmap.OfflineLoaded() && m_badmap.IsBad(ofl))
      {
        ++m_nDeadCells;
      
        if (Verbosity() >= VERBOSITY_MORE)
        {
          std::cout << PHWHERE << " - dead strip at layer " << layer << ": ";
          hit->identify();
        }
      
        dead_hits.insert(hit_iter->first);  // store hitkey of dead channels to be remove later
        continue;
      }

      if (_energy_scale.count(layer) > 1)
      {
        assert(!"Error: _energy_scale has two or more keys.");
      }
      const float mip_e = _energy_scale[layer];

      //      std::vector<double>& vadcrange = _max_fphx_adc[layer];
      // for(int i=0;i<8;i++) std::cout<<"Digitizer:: vadcrange= "<<vadcrange[i]<<std::endl;

      // c++ upper_bound finds the bin location above the test value (or vadcrange.end() if there isn't one)
      //      auto irange = std::upper_bound(vadcrange.begin(), vadcrange.end(),
      //          hit->getEnergy() / (TrkrDefs::InttEnergyScaleup * (double) mip_e));
      //          hit->getEnergy() / 100.0);
      //      int adc = (irange-vadcrange.begin())-1;
      //      if (adc == -1) adc = 0;

      double k = 85.7 / (TrkrDefs::InttEnergyScaleup * (double) mip_e);
      double E = hit->getEnergy() * k;  // keV

      double gain = 100.0;
      double offset = 280.0;
      double para = 1.0;
      double e_vol = (E * pow(10, 3) * 1.6 * pow(10, -19) * pow(10, 15) * gain / 3.6) + offset;
      double v_dac = para * (e_vol - 210.0) / 4.0;

      if (v_dac < 30)
      {
        v_dac = 15;
      }
      else if (v_dac < 60)
      {
        v_dac = 30;
      }
      else if (v_dac < 90)
      {
        v_dac = 60;
      }
      else if (v_dac < 120)
      {
        v_dac = 90;
      }
      else if (v_dac < 150)
      {
        v_dac = 120;
      }
      else if (v_dac < 180)
      {
        v_dac = 150;
      }
      else if (v_dac < 210)
      {
        v_dac = 180;
      }
      else
      {
        v_dac = 210;
      }

      hit->setAdc(v_dac);
      /*
            std::cout<<"Digitizer:: getEnergy = "<<hit->getEnergy()<<std::endl;
            std::cout<<"Digitizer:: Energy = "<<E<<std::endl;
            std::cout<<"Digitizer:: k = "<<k<<std::endl;
            //std::cout<<"Digitizer:: capa = "<<capa<<std::endl;
            std::cout<<"Digitizer:: E2V = "<<e_vol<<std::endl;
            std::cout<<"Digitizer:: V2DAC = "<<v_dac<<std::endl;
            //std::cout<<"Digitizer:: mip_e = "<<TrkrDefs::InttEnergyScaleup * (double) mip_e<<std::endl;
            //std::cout<<"Digitizer:: getE/mip_e = "<<hit->getEnergy() /(TrkrDefs::InttEnergyScaleup * (double) mip_e)<<std::endl;
            //std::cout<<"Digitizer:: InttEnergyScaleup = "<<TrkrDefs::InttEnergyScaleup<<std::endl;
      */
      // if(adc==0) adc=15;
      // else adc=adc*30;
      // hit->setAdc(adc);
      // std::cout<<"Digitizer:: adc= "<<adc<<std::endl;

      if (Verbosity() > 2)
      {
        std::cout << "PHG4InttDigitizer: found hit with layer " << layer << " ladder_z " << ladder_z << " ladder_phi " << ladder_phi
                  << " strip_col " << strip_col << " strip_row " << strip_row << " adc " << hit->getAdc() << std::endl;
      }
    }  // end loop over hits in this hitset

    // remove hits on dead channel in TRKR_HITSET and TRKR_HITTRUTHASSOC
    for (const auto &key : dead_hits)
    {
      if (Verbosity() > 2)
      {
        std::cout << " PHG4InttDigitizer: remove hit with key: " << key << std::endl;
      }
      hitset->removeHit(key);

      if (hittruthassoc)
      {
        hittruthassoc->removeAssoc(hitsetkey, key);
      }
    }
  }  // end loop over hitsets

  return;
}

//! end of process
int PHG4InttDigitizer::End(PHCompositeNode * /*topNode*/)
{
  if (Verbosity() >= VERBOSITY_SOME)
  {
    std::cout << "PHG4InttDigitizer::End - processed "
              << m_nCells << " cell with "
              << m_nDeadCells << " dead cells masked"
              << " (" << 100. * m_nDeadCells / m_nCells << "%)" << std::endl;
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

void PHG4InttDigitizer::SetDefaultParameters()
{
  set_default_double_param("NoiseMean", 457.2);
  set_default_double_param("NoiseSigma", 166.6);
  set_default_double_param("EnergyPerPair", 3.62e-9);  // GeV/e-h
  return;
}

float PHG4InttDigitizer::added_noise()
{
  //  float noise = gsl_ran_gaussian(RandomGenerator, mNoiseSigma) + mNoiseMean;
  //  noise = (noise < 0) ? 0 : noise;

  // Note the noise is bi-polar, i.e. can make ths signal fluctuate up and down.
  // Much of the mNoiseSigma as extracted in https://github.com/sPHENIX-Collaboration/coresoftware/pull/580
  // is statistical fluctuation from the limited calibration data. They does not directly apply here.
  float noise = gsl_ran_gaussian(RandomGenerator, mNoiseMean);

  return noise;
}

void PHG4InttDigitizer::set_adc_scale(const int &layer, std::vector<double> userrange)
{
  if (userrange.size() != nadcbins)
  {
    std::cout << "Error: vector in set_fphx_adc_scale(vector) must have eight elements." << std::endl;
    gSystem->Exit(1);
  }
  std::sort(userrange.begin(), userrange.end());
  _max_fphx_adc.insert(std::make_pair(layer, userrange));
}
