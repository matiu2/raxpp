/// Contains models for Monitoring related objects
#pragma once

#include <string>
#include <iostream>

namespace raxpp {
namespace model {

struct MonitoringZone {
  std::string countryCode;
  std::string id;
  std::string label;
  std::string sourceIPRangeV4;
  std::string sourceIPRangeV6;
};

/// Helps to print them out if needed
std::ostream &operator<<(std::ostream &s, MonitoringZone &z);

}
}

