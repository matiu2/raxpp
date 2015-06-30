#include "Rackspace.hpp"

#include <parse_to_json_class.hpp>

#include "utils.hpp"

namespace raxpp {
namespace api {

std::string Rackspace::login(const std::string& username, const std::string& apikey) {
  using namespace json;
  json::JSON outgoing(
      JMap{{"auth", JMap{{"RAX-KSKEY:apiKeyCredentials",
                          JMap{{"username", username}, {"apiKey", apikey}}}}}});
  /// Connect
  std::stringstream stream;
  std::string reply;
  stream << outgoing;
  client.url("https://auth.api.rackspacecloud.com/v2.0/tokens")
#ifdef DEBUG_CURL
      .setOpt(CURLOPT_VERBOSE)
#endif
      .header("Accept: application/json")        // These hearders will stay on the connection forever
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

curl::Easy& Rackspace::request(const std::string& url) {
  client.reset();
  return client.url(url.c_str())
#ifdef DEBUG_CURL
      .setOpt(CURLOPT_VERBOSE)
#endif
      .header(std::string("X-Auth-Token: ") + _token)
      .header("Accept: application/json")
      .header("Content-type: application/json");
}

json::JSON Rackspace::get(const std::string& url) {
  std::string reply;
  request(url).perform(reply);
  return json::readValue(reply.begin(), reply.end());
}

int Rackspace::del(const std::string& url) {
  client.reset();
  return client.url(url.c_str())
      .header(std::string("X-Auth-Token: ") + _token)
      .DELETE()
      .perform()
      .responseCode();
}

}
}
