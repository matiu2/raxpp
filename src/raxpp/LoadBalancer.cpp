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
  auto item = itemsToDelete.cbegin();
  std::vector<int> batch;
  // We have to delete in batches of 10 or the API freaks out
  for (int i=0; i<10; ++i)  {
  }
}
}
