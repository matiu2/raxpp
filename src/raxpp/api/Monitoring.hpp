#pragma once

#include <map>
#include <vector>

#include <raxpp/base/Datacenter.hpp>
#include <raxpp/api/Rackspace.hpp>

namespace raxpp {
namespace api {

class Monitoring {
private:
  Rackspace& rs;
  std::string url;
public:
  Monitoring(Rackspace& rs);
  /// List the zones (datacenters) where you can do remote monitoring from.
  ///
  /// Example output:
  /// {
  ///    "values": [
  ///        {
  ///            "id": "mzdfw",
  ///            "label": "dfw",
  ///            "country_code": "US",
  ///            "source_ips": [
  ///                "2001:4800:7902:0001::/64",
  ///                "50.56.142.128/26"
  ///            ]
  ///        },
  ///        { "id": "mzhkg", "label": "hkg", "country_code": "HK", "source_ips": [ "180.150.149.64/26", "2401:1800:7902:1:0:0:0:0/64" ] },
  ///        { "id": "mzlon", "label": "lon", "country_code": "GB", "source_ips": [ "2a00:1a48:7902:0001::/64", "78.136.44.0/26" ] },
  ///        { "id": "mzord", "label": "ord", "country_code": "US", "source_ips": [ "2001:4801:7902:0001::/64", "50.57.61.0/26" ] }
  ///    ],
  ///    "metadata": {
  ///        "count": 4,
  ///        "limit": 100,
  ///        "marker": null,
  ///        "next_href": null
  ///    }
  /// }
  json::JMap listMonitoringZones();
};

}
}
