#include "Monitoring.hpp"

#include "HTTPCodeHandler.hpp"
#include <sstream>
#include <stdexcept>


namespace raxpp {
namespace api {

Monitoring::Monitoring(Rackspace &rs) : rs(rs) {
  const json::JList &endpoints = rs.getCatalog("cloudMonitoring").at("endpoints");
  if (endpoints.size() != 1) {
    std::stringstream msg;
    msg << "Asked for the endpoint for cloudMonitoring, expected a list with one result, but got: "
        << endpoints;
    throw std::runtime_error(msg.str());
  }
  url = endpoints[0].at("publicURL");
}

/// Used to check the http response codes and throw exceptions if necessary
HTTPCodeHandler const check_monitoring_read{
    {200}, // Good codes
    {
     {400, "Monitoring Fault / Bad Request"},
     {500, "Monitoring Fault"},
     {503, "Service Unavailable"},
     {401, "Unauthorized - Check authentication token"},
     {413, "Over Limit - Wait, and try later"},
    }};

json::JList Monitoring::listMonitoringZones() const {
    json::JMap result = rs.get(url + "/monitoring_zones", check_monitoring_read);
    // TODO: result.at("metadata") contains { "count": 4, "limit": 100, "marker": null, "next_href": null }
    // We might want to actually process that
    return result.at("values");
}


}
}
