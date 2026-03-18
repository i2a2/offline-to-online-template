#ifndef PREPROCESS_CPP_CONFIG_H
#define PREPROCESS_CPP_CONFIG_H

#pragma once
#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

// Used as third party library for JSON parsing. Header-only, so no linking needed.
#include <nlohmann/json.hpp>

class config {

    struct MetaCfg {
        std::string author;
        std::string version = "1.0";
        std::string license;
        std::string copyright;
    };

    struct TrigCfg {
        int n = 64;
        int start_k = 0;
    };

    struct HeaderSpec {
        std::string file;
        std::string description;

        std::map<std::string, int64_t> ints;
        std::map<std::string, double> floats;

        std::vector<std::string> emit;
    };

    struct Config {
        int shot = -1;
        std::string diag;
        int core_edge_id = 0;
        std::filesystem::path output_dir;

        MetaCfg meta;
        TrigCfg trig;

        std::vector<HeaderSpec> headers;
    };

    static nlohmann::json load_json_file(const std::filesystem::path &path);

public:
    static Config load_config_json(const std::filesystem::path& path);
};


#endif //PREPROCESS_CPP_CONFIG_H

