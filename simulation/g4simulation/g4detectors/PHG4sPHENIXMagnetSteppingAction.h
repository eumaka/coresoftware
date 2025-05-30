// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef G4DETECTORS_PHG4SPHENIXMAGNETSTEPPINGACTION_H
#define G4DETECTORS_PHG4SPHENIXMAGNETSTEPPINGACTION_H

#include <g4main/PHG4SteppingAction.h>

class G4Step;
class G4VPhysicalVolume;
class PHCompositeNode;
class PHG4sPHENIXMagnetDetector;
class PHG4Hit;
class PHG4HitContainer;
class PHParameters;
class PHG4Shower;

class PHG4sPHENIXMagnetSteppingAction : public PHG4SteppingAction
{
 public:
  //! constructor
  PHG4sPHENIXMagnetSteppingAction(PHG4sPHENIXMagnetDetector *, const PHParameters *parameters);

  //! destructor
  ~PHG4sPHENIXMagnetSteppingAction() override;

  //! stepping action
  bool UserSteppingAction(const G4Step *, bool) override;

  //! reimplemented from base class
  void SetInterfacePointers(PHCompositeNode *) override;

 private:
  //! pointer to the detector
  PHG4sPHENIXMagnetDetector *m_Detector;
  const PHParameters *m_Params;
  //! pointer to hit container
  PHG4HitContainer *m_HitContainer;
  PHG4Hit *m_Hit;
  PHG4Shower *m_SaveShower;
  G4VPhysicalVolume *m_SaveVolPre;
  G4VPhysicalVolume *m_SaveVolPost;
  int m_SaveTrackId;
  int m_SavePreStepStatus;
  int m_SavePostStepStatus;
  int m_ActiveFlag;
  int m_BlackHoleFlag;

  int m_UseG4StepsFlag;
};

#endif  //__G4PHPHYTHIAREADER_H__
