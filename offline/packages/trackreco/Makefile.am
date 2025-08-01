##############################################
# please add new classes in alphabetical order

AUTOMAKE_OPTIONS = foreign

# List of shared libraries to produce
lib_LTLIBRARIES = \
  libtrack_reco.la

AM_CPPFLAGS = \
  -DRAVE -DRaveDllExport= \
  -I$(includedir) \
  -isystem$(OFFLINE_MAIN)/include \
  -isystem${G4_MAIN}/include \
  -isystem$(ROOTSYS)/include \
  -fopenmp

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OFFLINE_MAIN)/lib64

pkginclude_HEADERS = \
  ActsAlignmentStates.h \
  ActsEvaluator.h \
  ActsPropagator.h \
  ALICEKF.h \
  AssocInfoContainer.h \
  AssocInfoContainerv1.h \
  DSTClusterPruning.h \
  GPUTPCBaseTrackParam.h \
  GPUTPCTrackLinearisation.h \
  GPUTPCTrackParam.h \
  MakeActsGeometry.h \
  MakeSourceLinks.h \
  nanoflann.hpp \
  PHActsGSF.h \
  PHActsKDTreeSeeding.h \
  PHActsSiliconSeeding.h \
  PHActsVertexPropagator.h \
  PHActsTrkFitter.h \
  PHActsTrackProjection.h \
  PHActsTrackPropagator.h \
  PHCASeeding.h \
  PHCASiliconSeeding.h \
  AzimuthalSeeder.h \
  PHCosmicsFilter.h \
  PHLineLaserReco.h \
  PHCosmicsTrkFitter.h \
  PHCosmicSeeder.h \
  PHCosmicTrackMerger.h \
  PHCosmicSeedCombiner.h \
  PHCosmicSiliconPropagator.h \
  PHGenFitTrkFitter.h \
  PHGhostRejection.h \
  PHInitVertexing.h \
  PHMicromegasTpcTrackMatching.h \
  PHSiliconTpcTrackMatching.h \
  PHSiliconCosmicSeeding.h \
  PHSimpleVertexFinder.h \
  PHTrackPruner.h \
  PHTrackCleaner.h \
  PHTrackSelector.h \
  PHRaveVertexing.h \
  PHSiliconHelicalPropagator.h \
  PHSiliconSeedMerger.h \
  PHSimpleKFProp.h \
  PHStreakFinder.h \
  PHTpcClusterMover.h \
  PHTpcDeltaZCorrection.h \
  PHTrackClusterAssociator.h \
  PHTrackPropagating.h \
  PHTrackSeeding.h \
  PHTrackFitting.h \
  PHTrackSetMerging.h \
  PHTrackSetCopyMerging.h \
  PHTruthClustering.h \
  PHTruthTrackSeeding.h \
  PHTruthSiliconAssociation.h \
  PHTruthVertexing.h \
  PrelimDistortionCorrection.h \
  PrelimDistortionCorrectionAuAu.h \
  SecondaryVertexFinder.h \
  SvtxTrackStateRemoval.h \
  TrackingIterationCounter.h \
  TpcSeedFilter.h

ROOTDICTS = \
  AssocInfoContainer_Dict.cc \
  AssocInfoContainerv1_Dict.cc

  pcmdir = $(libdir)
  nobase_dist_pcm_DATA = \
  AssocInfoContainer_Dict_rdict.pcm \
  AssocInfoContainerv1_Dict_rdict.pcm

ACTS_SOURCES = \
  ActsAlignmentStates.cc \
  ActsEvaluator.cc \
  ActsPropagator.cc \
  MakeActsGeometry.cc \
  MakeSourceLinks.cc \
  PHActsGSF.cc \
  PHActsKDTreeSeeding.cc \
  PHActsSiliconSeeding.cc \
  PHActsTrkFitter.cc \
  PHActsVertexPropagator.cc \
  PHActsTrackProjection.cc \
  PHActsTrackPropagator.cc \
  PHCosmicsTrkFitter.cc

$OFFLINE_MAIN/share:
dist_data_DATA = \
  tgeo-sphenix.json \
  tgeo-sphenix-mms.json

AM_CPPFLAGS += -I$(OFFLINE_MAIN)/include/ActsFatras

ACTS_LIBS = \
  -lActsCore \
  -lActsPluginTGeo \
  -lActsExamplesDetectorTGeo \
  -lActsExamplesFramework

libtrack_reco_la_SOURCES = \
  $(ACTS_SOURCES) \
  ALICEKF.cc \
  DSTClusterPruning.cc \
  PH3DVertexing.cc \
  PHCASeeding.cc \
  PHCASiliconSeeding.cc \
  AzimuthalSeeder.cc \
  GPUTPCTrackParam.cxx \
  PHCosmicsFilter.cc \
  PHLineLaserReco.cc \
  PHCosmicSeedCombiner.cc \
  PHCosmicSeeder.cc \
  PHCosmicTrackMerger.cc \
  PHCosmicSiliconPropagator.cc \
  PHGenFitTrkFitter.cc \
  PHGhostRejection.cc \
  PHInitVertexing.cc \
  PHMicromegasTpcTrackMatching.cc \
  PHRaveVertexing.cc \
  PHSiliconHelicalPropagator.cc \
  PHSiliconSeedMerger.cc \
  PHSiliconTpcTrackMatching.cc \
  PHSiliconCosmicSeeding.cc \
  PHSimpleKFProp.cc \
  PHSimpleVertexFinder.cc \
  PHStreakFinder.cc \
  PHTpcClusterMover.cc \
  PHTpcDeltaZCorrection.cc \
  PHTrackPruner.cc \
  PHTrackCleaner.cc \
  PHTrackClusterAssociator.cc \
  PHTrackSeeding.cc \
  PHTrackSelector.cc \
  PHTrackSetMerging.cc \
  PHTrackPropagating.cc \
  PHTrackFitting.cc \
  PHTruthClustering.cc \
  PHTruthTrackSeeding.cc \
  PHTruthVertexing.cc \
  PHTruthSiliconAssociation.cc \
  PrelimDistortionCorrection.cc \
  PrelimDistortionCorrectionAuAu.cc \
  SecondaryVertexFinder.cc \
  SvtxTrackStateRemoval.cc \
  TrackingIterationCounter.cc \
  TpcSeedFilter.cc

libtrack_reco_la_LIBADD = \
  -lActsCore \
  -lActsPluginTGeo \
  -lActsExamplesDetectorTGeo \
  -lActsExamplesFramework \
  -lcalo_io \
  -lg4eval \
  -lg4testbench \
  -lg4detectors \
  -lg4tpc \
  -lg4intt \
  -lg4mvtx \
  -lgenfit2 \
  -lgenfit2exp \
  -lglobalvertex_io \
  -lgsl \
  -lgslcblas \
  -lintt \
  -lintt_io \
  -lmicromegas_io \
  -lmvtx \
  -lmvtx_io \
  -lphparameter_io \
  -lPHGenFit \
  -lSubsysReco \
  -ltrack_io \
  -ltpc \
  -ltrackbase_historic


# Rule for generating table CINT dictionaries.
%_Dict.cc: %.h %LinkDef.h
	rootcint -f $@ @CINTDEFS@ $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

#just to get the dependency
%_Dict_rdict.pcm: %_Dict.cc ;


################################################
# linking tests

BUILT_SOURCES = \
  testexternals.cc

noinst_PROGRAMS = \
  testexternals_track_reco


testexternals_track_reco_SOURCES = testexternals.cc
testexternals_track_reco_LDADD = libtrack_reco.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

##############################################
# please add new classes in alphabetical order

clean-local:
	rm -f *Dict* $(BUILT_SOURCES) *.pcm
