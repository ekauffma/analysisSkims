import os
import datetime
from CRABClient.UserUtilities import config
config = config()

todaysDate = datetime.date.today().strftime('%Y%m%d')

config.General.requestName = f'CICADAAnalysis_ZeroBias_2024G_{todaysDate}'
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.workArea = '/afs/hep.wisc.edu/home/ekauffma/test-dir/crabWorkArea'

config.JobType.allowUndistributedCMSSW = True
config.JobType.psetName = f'{os.environ["CMSSW_BASE"]}/src/anomalyDetection/analysisSkims/python/makeAnalysisNtuples_cfg.py'
config.JobType.pluginName = 'Analysis'
config.Data.outLFNDirBase = '/store/user/ekauffma/'

config.JobType.maxMemoryMB = 5000

config.Data.inputDataset = '/ZeroBias/ekauffma-CICADAAnalysis_ZeroBias_Skim_2024G_20240926-080cfef1d132d8bceee20e862ebc4f98/USER'
config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 10000
config.Data.publication = False
config.Data.outputDatasetTag = f'CICADAAnalysis_ZeroBias_2024G_{todaysDate}'
config.Data.partialDataset = True

config.Site.storageSite = 'T2_US_Wisconsin'
