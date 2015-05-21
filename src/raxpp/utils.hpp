/// Some useful utility functions
#pragma once

#include <stdexcept>

#include <json_class.hpp>

namespace raxpp {

/**
* @brief Finds a 
*
* Iterates through a json list of json map/dicts. Returns map that contains the @attribute with @value
*
* @param input A json list of jsonmaps
* @param attribute an attribute name that we're looking for in the map
* @param value the attribute value that we want
*
* @return the first json map that matches
* @throws std::runtime_error if it can't find a match
*/
inline const json::JMap &findInList(const json::JList &input,
                                    const std::string &attribute,
                                    const std::string &value) {
  for (const json::JSON &j : input)
    if (j.at(attribute) == value)
      return j;
  throw std::runtime_error(std::string("Expected to find ") + attribute +
                           " but it was not found");
}
}
