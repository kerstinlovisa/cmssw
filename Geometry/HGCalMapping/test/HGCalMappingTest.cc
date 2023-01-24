#include "Geometry/HGCalMapping/interface/HGCalCellLocator.h"
#include <iostream>
#include <cassert>
#include <string>
#include <chrono>
#include <random>


void testCellLocator()
{
    std::cout << "Testing of HGCalCellLocator class" << std::endl;

    HGCalCellLocator locator;

    uint16_t fedid = 0;
    uint8_t captureblock(0), econdidx(0), econderx(0), halfrocch(16);
    HGCalElectronicsId eid(fedid, captureblock, econdidx, econderx, halfrocch);

    int minlayer = 34;
    int maxlayer = 47;
    int maxiphi = 12;
    int nrings, layer, ring, iphi;
    int modulelayer,modulering,moduleiphi,cellring,celliphi;
    
    for (layer = minlayer; layer <= maxlayer; layer++) {
        nrings = ((layer <= 37) ? 2 : ((layer <= 43) ? 4 : 5));
        for (ring = 0; ring < nrings; ring++) {
            for (iphi = 0; iphi < maxiphi; iphi++) {
                std::string type = locator.getModuleType(layer,ring,iphi);
                std::tie(modulelayer,modulering,moduleiphi) = locator.getModuleLocation(layer,type);
                assert(modulelayer == layer);
                assert(modulering == ring);

                std::tie(cellring,celliphi) = locator.getCellLocation(econderx, halfrocch, modulelayer, modulering, moduleiphi);
                float halfrocch_ = locator.getHalfrocChannel(type, cellring, celliphi);
                assert((int)halfrocch_ == (int)halfrocch);

                std::tie(cellring,celliphi) = locator.getCellLocation(eid, modulelayer, modulering, moduleiphi);
                halfrocch_ = locator.getHalfrocChannel(type, cellring, celliphi);
                assert((int)halfrocch_ == (int)eid.halfrocChannel());
            }
        }
    }

    std::cout << "HGCalCellLocator done" << std::endl;
}

int main() {
    testCellLocator();
    return 0;
}