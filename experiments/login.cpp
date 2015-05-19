/// Logis in to Rackspace and shows the token and the load balancer API

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

#include <json_class.hpp>
#include <parse_to_json_class.hpp>
#include <curlpp11.hpp>

int main(int argc, char** argv)
{
  // Get the username and api key
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " USERNAME API_KEY" << std::endl;
    return -1;
  }
  std::string username = argv[1];
  std::string key = argv[2];
  // json should be { "auth":{ "RAX-KSKEY:apiKeyCredentials":{ "username":"${USER}", "apiKey":"${KEY}" } } }
  using namespace json;
  JSON login(
      JMap{{"auth", JMap{{"RAX-KSKEY:apiKeyCredentials",
                          JMap{{"username", username}, {"apiKey", key}}}}}});
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
  JSON data = json::readValue(reply.begin(), reply.end());
  std::string token = data.at("access").at("token").at("id");
  std::cout << "Token: " << token << std::endl;
  // Get the load balancer URL
  JList& catalog = data.at("access").at("serviceCatalog");
  auto findMap = [](JList& input, const std::string& attribute, const std::string& value) -> JSON& {
    for(JSON& j : input)
      if (j.at(attribute) == value)
        return j;
    throw std::runtime_error(std::string("Expected to find ") + attribute + " but it was not found");
  };
  JMap& lbs = findMap(catalog, "name", "cloudLoadBalancers");
  JList& endpoints = lbs.at("endpoints");
  JMap& ord = findMap(endpoints, "region", "ORD");
  std::string url = ord.at("publicURL");
  std::cout << "ORD Load balancers URL: " << url << std::endl;
  return 0;
}
