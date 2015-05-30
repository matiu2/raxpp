#pragma once

#include <string>

#include "Rackspace.hpp"

namespace raxpp {

/**
* @brief Log in to rackspace (US)
*
* @param username rackspace username
* @param apikey rackspace apikey
*
* @return The rackspace object that can be used to get all the other services
*/
Rackspace login(const std::string& username, const std::string& apikey);

}
