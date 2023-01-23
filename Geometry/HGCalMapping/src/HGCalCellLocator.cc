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
            CellToLoc_[std::make_pair(index,type)] = std::make_pair(u,v);
            LocToCell_[std::make_pair(u,v)] = std::make_pair(index,type);
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
            std::pair<int,std::string> moduletype (layer, type);
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

std::string HGCalCellLocator::getModuleType(int modulelayer, int modulering, int moduleiphi) const
{
    std::tuple<int,int,int> moduleloc (modulelayer, modulering, moduleiphi);
    std::string type;
    std::tie(modulelayer, type) = ModLocToType_.find(moduleloc)->second;
    return type;
}
std::string HGCalCellLocator::getCellType(int celllayer, int cellring, int celliphi) const
{
    std::pair<int,int> cellloc (cellring, celliphi);
    int cellidx;
    std::string type;
    std::tie(cellidx, type) = LocToCell_.find(cellloc)->second;
    return type;
}

std::tuple<int,int,int> HGCalCellLocator::getModuleLocation(int modulelayer, std::string type) const
{
    std::pair<int,std::string> moduletype (modulelayer, type);
    std::tuple<int,int,int> moduleloc = TypeToModLoc_.find(moduletype)->second;
    return moduleloc;
}

std::pair<int,int> HGCalCellLocator::getCellLocation(int econderx, int halfrocch, int modulelayer, int modulering, int moduleiphi) const
{
    std::string type = getModuleType(modulelayer, modulering, moduleiphi);
    std::pair<int,std::string> cell(halfrocch*2, type);
    std::pair<int,int> loc = CellToLoc_.find(cell)->second;
    return loc;
}

std::pair<int,int> HGCalCellLocator::getCellLocation(const HGCalElectronicsId& id, int modulelayer, int modulering, int moduleiphi) const
{
    int halfrocch = (int)id.halfrocChannel();
    std::string type = getModuleType(modulelayer, modulering, moduleiphi);

    std::pair<int,std::string> cell(halfrocch*2,type);
    std::pair<int,int> loc = CellToLoc_.find(cell)->second;
    return loc;
}

float HGCalCellLocator::getHalfrocChannel(int celllayer, int cellring, int celliphi) const
{
    std::pair<int,int> loc (cellring, celliphi);
    int halfrocch;
    std::string type;
    std::tie(halfrocch, type) = LocToCell_.find(loc)->second;
    return halfrocch/2;
}

