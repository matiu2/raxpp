#include "Datacenter.hpp"

namespace raxpp {
const std::map<Datacenter, std::string> dcNames{
    {DFW, "DFW"}, {HKG, "HKG"}, {IAD, "IAD"}, {ORD, "ORD"}, {SYD, "SYD"}};
const std::map<std::string, Datacenter> dcVals{
    {"DFW", DFW}, {"HKG", HKG}, {"IAD", IAD}, {"ORD", ORD}, {"SYD", SYD}};
}
