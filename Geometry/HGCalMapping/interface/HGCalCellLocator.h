#ifndef Geometry_HGCalMapping_HGCalCellLocator_H
#define Geometry_HGCalMapping_HGCalCellLocator_H 1

#include "DataFormats/HGCalDigi/interface/HGCalElectronicsId.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <tuple>
#include <iterator>

class HGCalCellLocator {
    public:

        HGCalCellLocator();

        /// Get type from module location
        std::string getModuleType(int modulelayer, int modulering, int moduleiphi) const;

        /// Get type from cell location
        std::string getCellType(int celllayer, int cellring, int celliphi) const;

        /// Get module location from type
        std::tuple<int,int,int> getModuleLocation(int modulelayer, std::string type) const;

        /// Get cell location from ROC fields and module location
        std::pair<int,int> getCellLocation(int econderx, int halfrocch, int modulelayer, int modulering, int moduleiphi) const;

        /// Get cell location (ring,iphi) from HGCalElectronicsId and module location
        std::pair<int,int> getCellLocation(const HGCalElectronicsId& id, int modulelayer, int modulering, int moduleiphi) const;

        /// Get cell index from cell location
        float getHalfrocChannel(int celllayer, int cellring, int celliphi) const;

    private:
        std::string channelfile_ = "src/Geometry/HGCalMapping/data/channels_sipmontile.hgcal.txt";
        std::string geometryfile_ = "src/Geometry/HGCalMapping/data/geometry_sipmontile.hgcal.txt";

        // (cell index, cell type) to (cell iring, cell iphi)
        std::map<std::pair<int,std::string>,std::pair<int,int> > CellToLoc_;
        std::map<std::pair<int,int>,std::pair<int,std::string> > LocToCell_;
        // (module layer, module iring, module iphi) to (module layer, module type)
        std::map<std::tuple<int,int,int>,std::pair<int,std::string> > ModLocToType_;
        std::map<std::pair<int,std::string>,std::tuple<int,int,int> > TypeToModLoc_;
};

#endif