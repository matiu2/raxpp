/// Logis in to Rackspace and shows the token and the load balancer API

#include <iostream>
#include <string>

#include <json_class.hpp>

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
  json::JSON login(
      JMap{{"auth", JMap{{"RAX-KSKEY:apiKeyCredentials",
                          JMap{{"username", username}, {"apiKey", key}}}}}});
  std::cout << login << std::endl;
  return 0;
}
