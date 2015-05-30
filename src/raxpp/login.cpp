#include "login.hpp"

#include <map>
#include <curlpp11.hpp>
#include <parse_to_json_class.hpp>
#include "utils.hpp"

namespace raxpp {

using json::readValue;

Rackspace login(const std::string& username, const std::string& apikey) {
  using namespace json;
  JSON login(
      JMap{{"auth", JMap{{"RAX-KSKEY:apiKeyCredentials",
                          JMap{{"username", username}, {"apiKey", apikey}}}}}});
  /// Connect
  std::stringstream stream;
  std::string reply;
  stream << login;
  curl::Easy client;
  client.url("https://auth.api.rackspacecloud.com/v2.0/tokens")
      //.setOpt(CURLOPT_VERBOSE)
      .header("Accept: application/json")
      .header("Content-type: application/json")
      .customBody(stream)
      .POST()
      .perform(reply);
  // Parse the response
  std::cout << "Resuponse Code: " << client.responseCode() << std::endl;
  return Rackspace(reply);
}

}
