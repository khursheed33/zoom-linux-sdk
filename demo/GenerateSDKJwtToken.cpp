#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "json.hpp" // nlohmann/json

using json = nlohmann::json;

// Simple SHA256 and HMAC-SHA256 implementation (public domain)
namespace SimpleCrypto {
    const uint32_t K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    uint32_t rotr(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }
    uint32_t ch(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (~x & z); }
    uint32_t maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (x & z) ^ (y & z); }
    uint32_t sigma0(uint32_t x) { return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22); }
    uint32_t sigma1(uint32_t x) { return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25); }
    uint32_t gamma0(uint32_t x) { return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3); }
    uint32_t gamma1(uint32_t x) { return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10); }

    std::vector<uint8_t> sha256(const std::string& input) {
        uint32_t h[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
        std::vector<uint8_t> data(input.begin(), input.end());
        uint64_t bit_len = data.size() * 8;

        data.push_back(0x80);
        while ((data.size() % 64) != 56) data.push_back(0);
        for (int i = 7; i >= 0; --i) data.push_back((bit_len >> (i * 8)) & 0xFF);

        for (size_t i = 0; i < data.size(); i += 64) {
            uint32_t w[64] = { 0 };
            for (int j = 0; j < 16; ++j) {
                w[j] = (data[i + j * 4] << 24) | (data[i + j * 4 + 1] << 16) | (data[i + j * 4 + 2] << 8) | data[i + j * 4 + 3];
            }
            for (int j = 16; j < 64; ++j) {
                w[j] = gamma1(w[j - 2]) + w[j - 7] + gamma0(w[j - 15]) + w[j - 16];
            }

            uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
            for (int j = 0; j < 64; ++j) {
                uint32_t t1 = hh + sigma1(e) + ch(e, f, g) + K[j] + w[j];
                uint32_t t2 = sigma0(a) + maj(a, b, c);
                hh = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
            }
            h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
        }

        std::vector<uint8_t> hash(32);
        for (int i = 0; i < 8; ++i) {
            hash[i * 4] = (h[i] >> 24) & 0xFF;
            hash[i * 4 + 1] = (h[i] >> 16) & 0xFF;
            hash[i * 4 + 2] = (h[i] >> 8) & 0xFF;
            hash[i * 4 + 3] = h[i] & 0xFF;
        }
        return hash;
    }

    std::vector<uint8_t> hmac_sha256(const std::string& key, const std::string& msg) {
        std::string o_key_pad(64, 0x5c), i_key_pad(64, 0x36);
        std::string k = key.length() > 64 ? std::string(sha256(key).begin(), sha256(key).end()) : key;
        k.resize(64, 0);

        for (size_t i = 0; i < 64; ++i) {
            o_key_pad[i] ^= k[i];
            i_key_pad[i] ^= k[i];
        }

        auto inner = sha256(i_key_pad + msg);
        return sha256(o_key_pad + std::string(inner.begin(), inner.end()));
    }
}

// Base64 URL encoding
std::string base64_url_encode(const std::vector<uint8_t>& input) {
    static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    int val = 0, valb = -6;
    for (uint8_t c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4) result.push_back('=');
    std::replace(result.begin(), result.end(), '+', '-');
    std::replace(result.begin(), result.end(), '/', '_');
    result.erase(std::remove(result.begin(), result.end(), '='), result.end());
    return result;
}

std::string base64_url_encode(const std::string& input) {
    return base64_url_encode(std::vector<uint8_t>(input.begin(), input.end()));
}

// Wrapper function to generate Zoom Meeting SDK JWT
std::string generateZoomJWT(const std::string& sdk_key, const std::string& sdk_secret,
    const std::string& meeting_number, int role, int expiration_seconds = 7200) {
    // Get current time (iat)
    auto iat = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    auto exp = expiration_seconds ? iat + expiration_seconds : iat + 7200; // Default to 2 hours if not specified

    // Create header
    json header = {
        {"alg", "HS256"},
        {"typ", "JWT"}
    };
    std::string s_header = header.dump();

    // Create payload
    json payload = {
        {"appKey", sdk_key},
        {"sdkKey", sdk_key},
        {"mn", meeting_number},
        {"role", role},
        {"iat", iat},
        {"exp", exp},
        {"tokenExp", exp}
    };
    std::string s_payload = payload.dump();

    // Encode header and payload
    std::string header_enc = base64_url_encode(s_header);
    std::string payload_enc = base64_url_encode(s_payload);
    std::string data = header_enc + "." + payload_enc;

    // Sign with HMAC-SHA256
    auto signature = SimpleCrypto::hmac_sha256(sdk_secret, data);
    std::string signature_enc = base64_url_encode(signature);

    // Combine into JWT
    return data + "." + signature_enc;
}

int main() {
    // Replace with your Zoom SDK credentials
    std::string sdk_key = "dF7fYaVThecj6jEPQq9Iw";    // From Zoom Developer Portal
    std::string sdk_secret = "NrPrP1ZyT55BVoDaZz4hHpISGBRLgZxT"; // From Zoom Developer Portal
    std::string meeting_number = "86143734334";  // e.g., "123456789"
    int role = 0;                                // 0 for participant, 1 for host
    int expiration_seconds = 7200;               // Default to 2 hours (optional)

    // Generate the JWT token using the wrapper
    std::string sdk_jwt = generateZoomJWT(sdk_key, sdk_secret, meeting_number, role, expiration_seconds);

    // Print the token and SDK key
    std::cout << "Zoom Meeting SDK JWT Token: " << sdk_jwt << std::endl;
    std::cout << "SDK Key: " << sdk_key << std::endl;

    return 0;
}