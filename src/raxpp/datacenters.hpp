#pragma once

#include <map>
#include <string>

namespace raxpp {

enum Datacenters = {DFW, HKG, IAD, ORD, SYD};
const std::map<Datacenter, std::string> dcNames;
const std::map<std::string, Datecenter> dcVals;

}
