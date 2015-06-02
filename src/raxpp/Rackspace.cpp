#include "Rackspace.hpp"

#include <json_class.hpp>
#include <parse_to_json_class.hpp>
#include <curlpp11.hpp>

#include "utils.hpp"

namespace raxpp {

std::string login(const std::string& username, const std::string& apikey) {
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
  if (!((client.responseCode() >= 200) && (client.responseCode() < 300))) {
    std::stringstream msg;
    msg << "HTTP response code: " << client.responseCode();
    throw std::runtime_error(msg.str());
  }
  return reply;
}

Rackspace::Rackspace(const std::string &username, const std::string &apikey) {
  auto reply = login(username, apikey);
  _json = json::readValue(reply.begin(), reply.end());
  _token = _json.at("access").at("token").at("id");
}

const json::JMap &Rackspace::getCatalog(const std::string &name) const {
  const json::JList &catalog = _json.at("access").at("serviceCatalog");
  return findInList(catalog, "name", name);
}
}
