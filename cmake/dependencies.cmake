
include(ExternalProject)

## Bandit (for tests)
ExternalProject_Add(bandit
    PREFIX 3rd_party
    GIT_REPOSITORY https://github.com/joakimkarlsson/bandit.git
    TLS_VERIFY true
    TLS_CAINFO certs/DigiCertHighAssuranceEVRootCA.crt
    UPDATE_COMMAND "" # Skip annoying updates for every build
    INSTALL_COMMAND ""
)
SET(BANDIT_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/src/bandit)

## jsonpp11 - JSON wrapper

ExternalProject_Add(jsonpp11
    PREFIX 3rd_party
    GIT_REPOSITORY https://github.com/matiu2/jsonpp11.git
    TLS_VERIFY true
    TLS_CAINFO certs/DigiCertHighAssuranceEVRootCA.crt
    BUILD_COMMAND make
    TEST_BEFORE_INSTALL 0
    TEST_COMMAND ctest
    UPDATE_COMMAND "" # Skip annoying updates for every build
    INSTALL_COMMAND ""
)
SET(JSONPP11_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/src/jsonpp11/src)


## curlpp11 - CURL wrapper

FIND_PACKAGE(CURL REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)

ExternalProject_Add(curlpp11_ext_proj
    PREFIX 3rd_party
    GIT_REPOSITORY https://github.com/matiu2/curlpp11.git
    TLS_VERIFY true
    TLS_CAINFO certs/DigiCertHighAssuranceEVRootCA.crt
    BUILD_COMMAND make
    TEST_BEFORE_INSTALL 0
    TEST_COMMAND ctest
    UPDATE_COMMAND "" # Skip annoying updates for every build
    INSTALL_COMMAND ""
)
SET(CURLPP11_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/src/curlpp11_ext_proj/src)
SET(CURLPP11_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/src/curlpp11_ext_proj-build/src)
add_library(curlpp11 STATIC IMPORTED)
set_target_properties(curlpp11 PROPERTIES IMPORTED_LOCATION ${CURLPP11_BUILD_DIR}/libcurlpp11.a)
