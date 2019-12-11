#ifndef CALORECO_RAWCLUSTERTOPO_H
#define CALORECO_RAWCLUSTERTOPO_H

#include <fun4all/SubsysReco.h>

#include <string>

class PHCompositeNode;
class RawClusterContainer;

class RawClusterBuilderTopo : public SubsysReco
{
 public:
  RawClusterBuilderTopo(const std::string &name = "RawClusterBuilderTopo");
  virtual ~RawClusterBuilderTopo() {}

  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

  void set_threshold_energy(const float e) { _min_tower_e = e; }

 private:
  void CreateNodes(PHCompositeNode *topNode);

  void set_noise( float noise_0, float noise_1 ) {
    _noise_LAYER[ 0 ] = noise_0;
    _noise_LAYER[ 1 ] = noise_1;
  }

  void set_significance( float seed, float grow, float peri ) {

    _sigma_seed = seed;
    _sigma_grow = grow;
    _sigma_peri = peri;

  }

  void allow_corner_neighbor( bool allow ) { 

    _allow_corner_neighbor = allow;

  }

  int get_ID( int ilayer, int ieta, int iphi ) {
    return ilayer * 24 * 64 + ieta * 64 + iphi;
  }
  
  int get_ilayer_from_ID( int ID ) {
    return ( (int) ( ID / ( 24 * 64 ) ) );
  }
  
  int get_ieta_from_ID( int ID ) {
    return ( (int) ( ( ID % ( 24 * 64 ) ) / ( 64 ) ) );
  }
  
  int get_iphi_from_ID( int ID ) {
    return ( (int) ( ID % 64 ) );
  }
  
  
  RawClusterContainer *_clusters;

  float _min_tower_e;

  float _TOWERMAP_E_LAYER_ETA_PHI[2][24][64];
  int _TOWERMAP_STATUS_LAYER_ETA_PHI[2][24][64];

  float _noise_LAYER[2];

  float _sigma_seed;
  float _sigma_grow;
  float _sigma_peri;

  bool _allow_corner_neighbor;

  std::string ClusterNodeName;
};

#endif
