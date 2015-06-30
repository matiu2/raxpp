#pragma once
#include <set>
#include <map>
#include <string>

namespace raxpp {
namespace api {

/// An exception thrown when the API returns something other than the expected error code
class APIError : public std::runtime_error {
public:
  int code;
  APIError(int code, const std::string& msg) : std::runtime_error(msg), code(code) {}
};

struct HTTPCodeHandler {
  std::set<int> goodCodes;
  std::map<int, std::string> errorResponses;
  /// Chceks the http response code and throws an exception if it's an error, on
  /// unkown response code
  void operator ()(int code, const std::string& context="") const;
};


}
}
