import os
import datetime
from CRABClient.UserUtilities import config
config = config()

todaysDate = datetime.date.today().strftime('%Y%m%d')

config.General.requestName = f'CICADAAnalysis_ZeroBias_Skim_2024G_{todaysDate}'
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.workArea = '/afs/hep.wisc.edu/home/ekauffma/test-dir/crabWorkArea'

config.JobType.allowUndistributedCMSSW = True
config.JobType.psetName = f'{os.environ["CMSSW_BASE"]}/src/anomalyDetection/analysisSkims/python/skimRAW.py'
config.JobType.pluginName = 'Analysis'
config.Data.outLFNDirBase = '/store/user/ekauffma/CICADASkims'
config.JobType.inputFiles=['/afs/hep.wisc.edu/user/ekauffma/ttbar_analysis_ntuples/CMSSW_14_1_0_pre3/src/anomalyDetection/CICADA']

config.JobType.maxMemoryMB = 5000
config.JobType.numCores = 1

config.Data.inputDataset = '/ZeroBias/Run2024G-PromptReco-v1/MINIAOD'
config.Data.secondaryInputDataset = '/ZeroBias/Run2024G-v1/RAW'
config.Data.runRange = '385281,385284,385285,385286,385290,385293,385294,385301,385303,385308,385310,385311,384981,385712,385713,385716,385719,385720,385722,385723,385724,385725,385727,385728,384981,384982,384985,384988,384990,384992,384993,384994,384995,384997,385000,385004,385010,385011,385012,385014,385015,385016,384191,384202,384203,384204,384207,384209,385713,385727,385728,385730,385731,385732,385733,385734,385735,385737,385738,384565'
config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 5500
config.Data.publication = True
config.Data.outputDatasetTag = f'CICADAAnalysis_ZeroBias_Skim_2024G_{todaysDate}'
config.Data.partialDataset = True

config.Site.storageSite = 'T2_US_Wisconsin'
