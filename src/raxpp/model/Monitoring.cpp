#include "Monitoring.hpp"

namespace raxpp {
namespace model {

std::ostream &operator<<(std::ostream &s, MonitoringZone &z) {
  s << "Monitoring Zone: country-code:" << z.countryCode << " id:" << z.id
    << " label:'" << z.label << "' source-ipv4-range:" << z.sourceIPRangeV4
    << " source-ipv6-range:" << z.sourceIPRangeV6 << std::endl;
  return s;
}

} /* model */
} /* raxpp */
