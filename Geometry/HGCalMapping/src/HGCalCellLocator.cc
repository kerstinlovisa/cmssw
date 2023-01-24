#include "Geometry/HGCalMapping/interface/HGCalCellLocator.h"

HGCalCellLocator::HGCalCellLocator()
{
    std::ifstream file;
    std::string line;
    int index, layer, u, v, iz;
    std::string type, trigsum, header;
    file.open(channelfile_.c_str());
    if (file.is_open())
    {
        file >> header >> header >> header >> header >> header >> header >> header;
        while (file >> index >> layer >> u >> v >> type >> iz >> trigsum)
        {
            CellToLoc_[std::make_tuple(type,index)] = std::make_tuple(type,u,v);
            LocToCell_[std::make_tuple(type,u,v)] = std::make_tuple(type,index);
        }
    }
    else
    {
        std::cout << "HGCalCellLocator::HGCalCellLocator : ROC channel mapping file can not be found in "
              << channelfile_.c_str() << std::endl;
    }
    file.close();


    file.open(geometryfile_.c_str(), std::ios::in);
    if (file.is_open())
    {
        getline(file, line);
        while(getline(file,line)) 
        {
            std::istringstream buffer(line);
            std::vector<std::string> arr((std::istream_iterator<std::string>(buffer)), std::istream_iterator<std::string>());
            index = atoi(arr[0].c_str());
            layer = atoi(arr[1].c_str());
            u = atoi(arr[2].c_str());
            v = atoi(arr[3].c_str());
            type = arr[4];
            std::tuple<int,int,int> moduleloc (layer,u,v);
            std::tuple<int,std::string> moduletype (layer, type);
            ModLocToType_[moduleloc] = moduletype;
            TypeToModLoc_[moduletype] = moduleloc;
        }
    }
    else
    {
        std::cout << "HGCalCellLocator::HGCalCellLocator : SiPM geometry mapping file can not be found in "
              << geometryfile_.c_str() << std::endl;
    }

    file.close();
}

std::string HGCalCellLocator::getModuleType(int layer, int modulering, int moduleiphi) const
{
    std::tuple<int,int,int> moduleloc (layer, modulering, moduleiphi);
    std::string type;
    std::tie(layer, type) = ModLocToType_.find(moduleloc)->second;
    return type;
}

std::tuple<int,int,int> HGCalCellLocator::getModuleLocation(int layer, std::string type) const
{
    std::tuple<int,std::string> moduletype (layer, type);
    std::tuple<int,int,int> moduleloc = TypeToModLoc_.find(moduletype)->second;
    return moduleloc;
}

std::pair<int,int> HGCalCellLocator::getCellLocation(int econderx, int halfrocch, int layer, int modulering, int moduleiphi) const
{
    std::string type = getModuleType(layer, modulering, moduleiphi);
    std::tuple<std::string,int> cell(type,halfrocch*2);
    int cellring, celliphi;
    std::tie(type,cellring,celliphi) = CellToLoc_.find(cell)->second;
    return std::make_pair(cellring,celliphi);
}

std::pair<int,int> HGCalCellLocator::getCellLocation(const HGCalElectronicsId& id, int layer, int modulering, int moduleiphi) const
{
    int halfrocch = (int)id.halfrocChannel();
    std::string type = getModuleType(layer, modulering, moduleiphi);
    std::tuple<std::string,int> cell(type,halfrocch*2);
    int cellring, celliphi;
    std::tie(type,cellring,celliphi) = CellToLoc_.find(cell)->second;
    return std::make_pair(cellring,celliphi);
}

float HGCalCellLocator::getHalfrocChannel(std::string type, int cellring, int celliphi) const 
{
    std::tuple<std::string,int,int> cellloc (type, cellring, celliphi);
    int rocch;
    std::tie(type, rocch) = LocToCell_.find(cellloc)->second;
    return (float)rocch/2;
}