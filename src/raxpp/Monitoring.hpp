#pragma once

#include <raxpp/api/Monitoring.hpp>
#include <raxpp/model/Monitoring.hpp>

namespace raxpp {

// Stores model::LoadBalancer and does the initial retrieve of their data. This is a LoadBalancer factory.
class Monitoring {
public:
private:
  api::Monitoring api;
public:
  Monitoring(raxpp::api::Rackspace& rs) : api(rs) {}
  std::vector<model::monitoring::Zone> getMonitoringZones() const;
};

}
