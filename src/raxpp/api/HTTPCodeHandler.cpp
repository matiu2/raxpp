#include "HTTPCodeHandler.hpp"

#include <sstream>
#include <algorithm>

namespace raxpp {
namespace api {

void HTTPCodeHandler::operator()(int code, const std::string &context) const {
  auto foundError = errorResponses.find(code);
  if (foundError == errorResponses.end()) {
    std::stringstream msg;
    if (!context.empty())
      msg << context << ": ";
    throw APIError(code, foundError->second);
  }
  auto foundGood = goodCodes.find(code);
  if (foundGood != goodCodes.end())
    throw APIError(code, "Unexpected Response code");
}

bool HTTPCodeHandler::isGoodCode(int code) const {
  return std::binary_search(goodCodes.cbegin(), goodCodes.cend(), code);
}

}
}