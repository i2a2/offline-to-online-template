#include "config.h"
#include <fstream>
#include <stdexcept>

using nlohmann::json;

json config::load_json_file(const std::filesystem::path &p) {
    std::ifstream f(p);
    if (!f) throw std::runtime_error("Failed to open config: " + p.string());
    json j;
    f >> j;
    return j;
}


config::Config config::load_config_json(const std::filesystem::path &path) {

    const json j = load_json_file(path);

    Config cfg;

    if (!j.contains("shot")) throw std::runtime_error("Config missing: shot");
    cfg.shot = j.at("shot").get<int>();

    cfg.diag = j.value("diag", "");
    cfg.core_edge_id = j.value("core_edge_id", 0);

    if (!j.contains("output_dir")) throw std::runtime_error("Config missing: output_dir");
    cfg.output_dir = j.at("output_dir").get<std::string>();

    // meta
    if (j.contains("meta")) {
        const auto& m = j.at("meta");
        cfg.meta.author = m.value("author", "");
        cfg.meta.version = m.value("version", "1.0");
        cfg.meta.license = m.value("license", "");
        cfg.meta.copyright = m.value("copyright", "");
    }

    // trig
    if (j.contains("trig")) {
        const auto& t = j.at("trig");
        cfg.trig.n = t.value("n", 64);
        cfg.trig.start_k = t.value("start_k", 0);
    }

    // headers
    if (!j.contains("headers") || !j.at("headers").is_array()) {
        throw std::runtime_error("Config missing/invalid: headers[]");
    }

    for (const auto& h : j.at("headers")) {
        HeaderSpec spec;
        spec.file = h.at("file").get<std::string>();
        spec.description = h.value("description", "");

        // constants
        if (h.contains("constants")) {
            const auto& c = h.at("constants");
            if (!c.is_object()) throw std::runtime_error("constants must be an object");

            for (auto it = c.begin(); it != c.end(); ++it) {
                const std::string key = it.key();
                const json& val = it.value();

                if (val.is_number_integer()) {
                    spec.ints[key] = val.get<int64_t>();
                } else if (val.is_number_float()) {
                    spec.floats[key] = val.get<double>();
                } else if (val.is_boolean()) {
                    spec.ints[key] = val.get<bool>() ? 1 : 0;
                } else {
                    throw std::runtime_error("Unsupported constant type for key: " + key);
                }
            }
        }

        // emit sections
        if (h.contains("emit")) {
            if (!h.at("emit").is_array()) throw std::runtime_error("emit must be an array of strings");
            for (const auto& e : h.at("emit")) {
                spec.emit.push_back(e.get<std::string>());
            }
        }

        cfg.headers.push_back(std::move(spec));
    }

    return cfg;
}


