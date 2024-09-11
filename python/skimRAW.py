import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_2023_cff import Run3_2023

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing('analysis')
options.parseArguments()

process = cms.Process("SKIM", Run3_2023)
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(options.maxEvents)
    input = cms.untracked.int32(100)
)
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                '/store/data/Run2024G/ZeroBias/MINIAOD/PromptReco-v1/000/383/805/00000/49008589-1bc6-4bae-b773-60e3edd6928a.root'
                            ),
                            secondaryFileNames = cms.untracked.vstring(
                                '/store/data/Run2024G/ZeroBias/RAW/v1/000/383/805/00000/1dd13c22-b18a-49b1-b2ab-63a86f32ff73.root'
                            )
)

process.options = cms.untracked.PSet(
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    deleteNonConsumedUnscheduledModules = cms.untracked.bool(True),
    dumpOptions = cms.untracked.bool(False),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(
            allowAnyLabel_=cms.required.untracked.uint32
        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(0)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(0),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

process.schedule = cms.Schedule()

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '130X_dataRun3_Prompt_v4', '')

#process.raw2digi_step = cms.Path(process.RawToDigi)
#process.schedule.append(process.raw2digi_step)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
#from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAW

#call to customisation function L1TReEmulFromRAW imported from L1Trigger.Configuration.customiseReEmul
#process = L1TReEmulFromRAW(process)

from anomalyDetection.analysisSkims.jetFilter_cfi import jetFilter
process.jetFilter = jetFilter
process.NJetPath = cms.Path(process.jetFilter)
process.schedule.append(process.NJetPath)

process.skimOutput = cms.OutputModule(
    'PoolOutputModule',
    fileName = cms.untracked.string(options.outputFile),
    outputCommands=cms.untracked.vstring(
        "keep *",
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('NJetPath')
    )
)

process.skimStep = cms.EndPath(
    process.skimOutput
)
process.schedule.append(process.skimStep)

print("schedule:")
print(process.schedule)
print("schedule contents:")
print([x for x in process.schedule])
