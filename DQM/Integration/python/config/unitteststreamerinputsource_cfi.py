from __future__ import print_function
from __future__ import absolute_import
from builtins import range
import os
import FWCore.ParameterSet.Config as cms

# Parameters for runType
import FWCore.ParameterSet.VarParsing as VarParsing
from .dqmPythonTypes import *

# Dedine and register options
options = VarParsing.VarParsing("analysis")

# Parameters for runType
options.register ('runkey',
          'pp_run',
          VarParsing.VarParsing.multiplicity.singleton,
          VarParsing.VarParsing.varType.string,
          "Run Keys of CMS")

# Parameter for frontierKey
options.register('runUniqueKey',
                 'InValid',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Unique run key from RCMS for Frontier")

options.register('runNumber',
                 356383,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Run number. This run number has to be present in https://github.com/cms-data/DQM-Integration")

options.register('streamLabel',
                 'streamDQM', # default DQM stream value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Name of the stream")

options.register('noDB',
                 True, # default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Don't upload the BeamSpot conditions to the DB")

options.register('scanOnce',
                 True, # default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Don't repeat file scans: use what was found during the initial scan. EOR file is ignored and the state is set to 'past end of run'.")

options.register('skipFirstLumis',
                 False, # default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Skip (and ignore the minEventsPerLumi parameter) for the files which have been available at the begining of the processing. ")

options.register('BeamSplashRun',
                 False, # default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Set client source settings for beam SPLASH run")

# This is used only by the online clients themselves. 
# We need to register it here because otherwise an error occurs saying that there is an unidentified option.
options.register('unitTest',
                 True,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Required to avoid the error.")

options.parseArguments()

# Read streamer files from https://github.com/cms-data/DQM-Integration
dqm_integration_data = [os.path.join(dir,'DQM/Integration/data') for dir in os.getenv('CMSSW_SEARCH_PATH','').split(":") if os.path.exists(os.path.join(dir,'DQM/Integration/data'))][0]
print("Reading streamer files from:\n ", dqm_integration_data)

# Set the process source
source = cms.Source("DQMStreamerReader",
    runNumber = cms.untracked.uint32(options.runNumber),
    runInputDir = cms.untracked.string(dqm_integration_data),
    SelectEvents = cms.untracked.vstring('*'),
    streamLabel = cms.untracked.string(options.streamLabel),
    scanOnce = cms.untracked.bool(options.scanOnce),
    minEventsPerLumi = cms.untracked.int32(1000),
    delayMillis = cms.untracked.uint32(500),
    nextLumiTimeoutMillis = cms.untracked.int32(0),
    skipFirstLumis = cms.untracked.bool(options.skipFirstLumis),
    deleteDatFiles = cms.untracked.bool(False),
    endOfRunKills  = cms.untracked.bool(False),
    inputFileTransitionsEachEvent = cms.untracked.bool(False)
)

maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

runType = RunType()
if not options.runkey.strip():
  options.runkey = "pp_run"

runType.setRunType(options.runkey.strip())
