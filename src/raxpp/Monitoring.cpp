#include "Monitoring.hpp"

#include <raxpp/model/Monitoring.hpp>
#include <raxpp/json-conversion/Monitoring.hpp>

namespace raxpp {

std::vector<model::monitoring::Zone> Monitoring::getZones() const {
  auto&& j = api.listZones();
  return json_conversion::json2monitoringZones(j);
}

std::vector<model::monitoring::Entity> Monitoring::getEntities() const {
  auto&& j = api.listEntities();
  return json_conversion::json2monitoringEntities(j);
}

}
