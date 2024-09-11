import FWCore.ParameterSet.Config as cms

jetFilter = cms.EDFilter(
    'jetFilter',
    objectSrc = cms.InputTag("slimmedJetsPuppi")
)
