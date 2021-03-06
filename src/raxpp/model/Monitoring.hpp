/// Contains models for Monitoring related objects
#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>

namespace raxpp {
namespace model {
namespace monitoring {

struct Zone {
  std::string countryCode;
  std::string id;
  std::string label;
  std::string sourceIPRangeV4;
  std::string sourceIPRangeV6;
};

/// Helps to print zones
std::ostream &operator<<(std::ostream &s, Zone &z);

struct Suppression {
  // TODO: fill this in
};

struct Entity {
  std::string uri;
  double updated_at;
  std::vector<Suppression> scheduled_suppressions;
  std::vector<Suppression> active_suppressions;
  std::string agent_id;
  double created_at;
  std::string id;
  /* eg. {
    "public1_v6": "2001:4802:...",
    "public0_v4": "104.130...",
    "private0_v4": "10.208...",
    "access_ip1_v4": "104.130...",
    "access_ip0_v6": "2001:4802:..."

    I can't seem to find documentation on the meaning of these
  },
  */
  std::map<std::string, std::string> ip_addresses;
  std::string label;
  bool managed;
  std::string metadata;
};


}
}
}
