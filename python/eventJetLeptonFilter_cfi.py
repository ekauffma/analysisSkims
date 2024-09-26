import FWCore.ParameterSet.Config as cms

eventJetLeptonFilter = cms.EDFilter(
    'eventJetLeptonFilter',
    objectSrc = cms.InputTag("slimmedJetsPuppi")
)
