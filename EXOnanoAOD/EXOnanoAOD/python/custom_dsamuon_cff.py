import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import *
from TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi import *
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *


DSAmuonsTable = cms.EDProducer("DSAMuonTableProducer",
    dsaMuons=cms.InputTag("displacedStandAloneMuons"),
    muons=cms.InputTag("linkedObjects","muons"),
    primaryVertex = cms.InputTag("offlineSlimmedPrimaryVertices"),
    beamspot = cms.InputTag("offlineBeamSpot")
)

DSAmuonVertexTable = cms.EDProducer("MuonVertexTableProducer",
    dsaMuons=cms.InputTag("displacedStandAloneMuons"),
    patMuons=cms.InputTag("linkedObjects","muons"),
    beamspot=cms.InputTag("offlineBeamSpot"),
    generalTracks=cms.InputTag("generalTracks"),
    primaryVertex=cms.InputTag("offlineSlimmedPrimaryVertices")
)

PATmuonExtendedTable = cms.EDProducer("MuonExtendedTableProducer",
    name=cms.string("Muon"),
    muons=cms.InputTag("linkedObjects","muons"),
    dsaMuons=cms.InputTag("displacedStandAloneMuons"),
    primaryVertex=cms.InputTag("offlineSlimmedPrimaryVertices"),
    beamspot=cms.InputTag("offlineBeamSpot"),
    generalTracks=cms.InputTag("generalTracks")
)

def add_dsamuonTables(process):

    process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
    process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi')

    process.DSAmuonsTable = DSAmuonsTable
    process.DSAmuonVertexTable = DSAmuonVertexTable
    process.PATmuonExtendedTable = PATmuonExtendedTable

    process.dsamuonTask = cms.Task(process.DSAmuonsTable)
    process.dsamuonVertexTask = cms.Task(process.DSAmuonVertexTable)
    process.patmuonTask = cms.Task(process.PATmuonExtendedTable)

    process.nanoTableTaskCommon.add(process.dsamuonTask)
    process.nanoTableTaskCommon.add(process.dsamuonVertexTask)
    process.nanoTableTaskCommon.add(process.patmuonTask)

    return process
