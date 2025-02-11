import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import *

genPartExtendedTable = cms.EDProducer("GenParticlesExtendedTableProducer",
    genparticles = cms.InputTag("finalGenParticles")
)

def add_genpartTables(process):
    process.genPartExtendedTable = genPartExtendedTable
    process.genPartExtendedTask = cms.Task(process.genPartExtendedTable)
    process.nanoTableTaskCommon.add(process.genPartExtendedTask)

    return process
