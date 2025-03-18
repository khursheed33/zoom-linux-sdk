// GenerateSDKJwtToken.h
#ifndef GENERATE_SDK_JWT_TOKEN_H
#define GENERATE_SDK_JWT_TOKEN_H

#include <string>
#include <vector>
#include "json.hpp" // Assuming nlohmann/json is available in your project

using json = nlohmann::json;

// Namespace for cryptographic functions
namespace SimpleCrypto {
    std::vector<uint8_t> sha256(const std::string& input);
    std::vector<uint8_t> hmac_sha256(const std::string& key, const std::string& msg);
}

// Base64 URL encoding functions
std::string base64_url_encode(const std::vector<uint8_t>& input);
std::string base64_url_encode(const std::string& input);

// Function to generate Zoom Meeting SDK JWT
std::string generateZoomJWT(const std::string& sdk_key, 
                           const std::string& sdk_secret,
                           const std::string& meeting_number, 
                           int role, 
                           int expiration_seconds = 7200);

#endif // GENERATE_SDK_JWT_TOKEN_H