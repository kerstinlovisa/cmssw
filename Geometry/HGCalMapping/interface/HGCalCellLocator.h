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
        std::string getModuleType(int layer, int modulering, int moduleiphi) const;

        /// Get module location from layer and module type
        std::tuple<int,int,int> getModuleLocation(int layer, std::string type) const;

        /// Get cell location from ROC fields and module location
        std::pair<int,int> getCellLocation(int econderx, int halfrocch, int layer, int modulering, int moduleiphi) const;

        /// Get cell location (ring,iphi) from HGCalElectronicsId and module location
        std::pair<int,int> getCellLocation(const HGCalElectronicsId& id, int layer, int modulering, int moduleiphi) const;

        float getHalfrocChannel(std::string type, int cellring, int celliphi) const;
    private:
        std::string channelfile_ = "src/Geometry/HGCalMapping/data/channels_sipmontile.hgcal.txt";
        std::string geometryfile_ = "src/Geometry/HGCalMapping/data/geometry_sipmontile.hgcal.txt";

        // (type, cell index) to (type, cell iring, cell iphi)
        std::map<std::tuple<std::string,int>,std::tuple<std::string,int,int> > CellToLoc_;
        std::map<std::tuple<std::string,int,int>,std::tuple<std::string,int> > LocToCell_;
        // (layer, module iring, module iphi) to (layer, type)
        std::map<std::tuple<int,int,int>,std::tuple<int,std::string> > ModLocToType_;
        std::map<std::tuple<int,std::string>,std::tuple<int,int,int> > TypeToModLoc_;
};

#endif