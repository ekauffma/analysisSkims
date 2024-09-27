import FWCore.ParameterSet.Config as cms

analysisObjectNtuplizer = cms.EDAnalyzer(
    'analysisObjectNtuplizer',
    objectSrc = cms.InputTag("slimmedJetsPuppi"),
    caloLayer1CICADAScore = cms.InputTag("caloLayer1Digis", "CICADAScore"),
)
