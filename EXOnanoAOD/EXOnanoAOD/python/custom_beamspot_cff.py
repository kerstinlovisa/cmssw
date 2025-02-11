import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import *

beamSpotTable = cms.EDProducer("BeamSpotTableProducer",
    beamSpot = cms.InputTag("offlineBeamSpot")
)

def add_beamspotTables(process):

    process.beamSpotTable = beamSpotTable
    process.beamSpotTask = cms.Task(process.beamSpotTable)
    process.nanoTableTaskCommon.add(process.beamSpotTask)

    return process
