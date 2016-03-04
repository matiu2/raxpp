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

model::monitoring::Entity json2monitoringEntity(const json::JMap &json) {
  /* "uri": "http...",
    "https://iad.servers.api.rackspacecloud.com/xxxxxx/servers/yyyyyyyy-zzzz-aaaa-aaaa-bbbbbbbbbbbb",
    "updated_at": 1.45246e+12,
    "scheduled_suppressions": [],
    "active_suppressions": [],
    "agent_id": "yyyyyyyy-zzzz-aaaa-aaaa-bbbbbbbbbbbb",
    "created_at": 1.45246e+12,
    "id": "enyTiVsoaw",
    "ip_addresses": {
      "public1_v6": "aaaa:bbbb:cccc:...",
      "public0_v4": "104.130...",
      "private0_v4": "10.208...",
      "access_ip1_v4": "104.130...",
      "access_ip0_v6": "2001:4802:..."
    },
    "label": "This is a server",
    "managed": true,
    "metadata": null
  }, */
  model::monitoring::Entity result;
  const auto& uri = json.at("uri");
  if (uri)
    result.uri = json.at("uri");
  result.updated_at = (double)json.at("updated_at");
  // result.scheduled_suppressions = json.at("scheduled_suppressions");
  // result.active_suppressions = json.at("active_suppressions");
  const auto& agent_id = json.at("agent_id");
  if (agent_id)
    result.agent_id = json.at("agent_id");
  result.created_at = (double)json.at("created_at");
  result.id = json.at("id");
  const auto& addresses_json = json.at("ip_addresses");
  if (addresses_json) {
    const json::JMap& addresses = addresses_json;
    for (const auto& row : addresses) {
      std::pair<std::string, std::string> entry;
      std::tie(entry.first, entry.second) = row;
      result.ip_addresses.insert(entry);
    }
  }
  const auto& label = json.at("label");
  if (label)
    result.label = json.at("label");
  result.managed = (bool)json.at("managed");
  const auto& metadata = json.at("metadata");
  if (metadata)
    result.metadata = metadata;
  return result;
}

std::vector<model::monitoring::Entity> json2monitoringEntities(const json::JList& json) {
  std::vector<model::monitoring::Entity> result;
  for (const auto& row : json)
    result.push_back(json2monitoringEntity(row));
  return result;
}

}
}

