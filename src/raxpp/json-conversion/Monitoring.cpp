#include "Monitoring.hpp"

#include <parse_to_json_class.hpp>

namespace raxpp {
namespace json_conversion {

std::vector<model::monitoring::Zone>
json2monitoringZones(const json::JList &json) {
  // Example: [{"country_code":"US","id":"mzdfw","label":"Dallas Fort Worth
  // (DFW)","source_ips":["2001:4800:7902:0001::/64","50.56.142.128/26"]},{"country_code":"HK","id":"mzhkg","label":"Hong
  // Kong
  // (HKG)","source_ips":["2401:1800:7902:0001::/64","180.150.149.64/26"]},{"country_code":"US","id":"mziad","label":"Northern
  // Virginia
  // (IAD)","source_ips":["2001:4802:7902:0001::/64","69.20.52.192/26"]},{"country_code":"GB","id":"mzlon","label":"London
  // (LON)","source_ips":["2a00:1a48:7902:0001::/64","78.136.44.0/26"]},{"country_code":"US","id":"mzord","label":"Chicago
  // (ORD)","source_ips":["2001:4801:7902:0001::/64","50.57.61.0/26"]},{"country_code":"AU","id":"mzsyd","label":"Sydney
  // (SYD)","source_ips":["2401:1801:7902:0001::/64","119.9.5.0/26"]}]
  std::vector<model::monitoring::Zone> result;
  // JSON item is a list
  for (const auto &item : json) {
    model::monitoring::Zone output;
    output.countryCode = item.at("country_code");
    output.id = item.at("id");
    output.label = item.at("label");
    // TODO: log and send a support ticket if we find that there are more than
    // two IPs
    const json::JList &ips = item.at("source_ips");
    for (const std::string &ip : ips) {
      if (ip.find(':') != std::string::npos)
        output.sourceIPRangeV6 = ip;
      else
        output.sourceIPRangeV4 = ip;
    }
    result.emplace_back(output);
  }
  return result;
}
}
}
