#pragma once

#include <map>
#include <string>

namespace raxpp {

// Keep NONE and ALL at the extremes as it's used for counting
enum Datacenter {DFW, HKG, IAD, ORD, SYD, ALL};
extern const std::map<Datacenter, std::string> dcNames;
extern const std::map<std::string, Datacenter> dcVals;

}
