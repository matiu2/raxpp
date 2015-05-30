#pragma once

#include <map>
#include <string>

namespace raxpp {

enum Datacenter {DFW, HKG, IAD, ORD, SYD};
extern const std::map<Datacenter, std::string> dcNames;
extern const std::map<std::string, Datacenter> dcVals;

}
