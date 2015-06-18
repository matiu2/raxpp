#pragma once

#include <map>
#include <string>

namespace raxpp {

// Keep NONE and ALL at the extremes as it's used for counting
enum Datacenter {DFW, HKG, IAD, ORD, SYD, ALL};
extern const std::map<Datacenter, std::string> dcNames;
extern const std::map<std::string, Datacenter> dcVals;
inline const std::string& dc2name(Datacenter dc) { return dcNames.at(dc); }
inline Datacenter name2dc(const std::string& name) { return dcVals.at(name); }

}
