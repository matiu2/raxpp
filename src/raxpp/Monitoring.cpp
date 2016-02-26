#include "Monitoring.hpp"

#include <raxpp/model/Monitoring.hpp>
#include <raxpp/json-conversion/Monitoring.hpp>

namespace raxpp {

std::vector<model::MonitoringZone> Monitoring::getMonitoringZones() const {
  auto&& j = api.listMonitoringZones();
  return json_conversion::json2monitoringZones(j);
}

}
