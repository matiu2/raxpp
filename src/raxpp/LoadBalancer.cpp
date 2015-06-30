#include "LoadBalancer.hpp"

#include <raxpp/json-conversion/LoadBalancer.hpp>

namespace raxpp {

AccessList& LoadBalancer::getAccessList(bool forceRefresh) {
  if (!accessList || forceRefresh) {
    auto json = api.getAccessList(model.dc, model.id);
    model.setAccessList(json_conversion::json2accessList(json));
    accessList.reset(new AccessList(api, *(model.accessList)));
  }
  return *accessList;
}

void AccessList::deleteItems(const std::vector<int> &itemsToDelete) {
  // TODO: Write this
}
}
