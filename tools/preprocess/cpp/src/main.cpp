#include "config.h"
#include "emit.h"
#include "provenance.hpp"
#include "generators/preprocess_hf0.h"

#include <filesystem>
#include <sstream>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

static std::string today_yyyy_mm_dd() {
    using clock = std::chrono::system_clock;
    auto now = clock::now();
    std::time_t t = clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

static std::string make_include_guard(const std::string& stem) {
    std::string out;
    out.reserve(stem.size() + 2);
    bool last_underscore = false;

    for (unsigned char ch : stem) {
        if (std::isalnum(ch)) {
            out.push_back(static_cast<char>(std::toupper(ch)));
            last_underscore = false;
        } else {
            if (!last_underscore) {
                out.push_back('_');
                last_underscore = true;
            }
        }
    }

    // trim underscores
    while (!out.empty() && out.front() == '_') out.erase(out.begin());
    while (!out.empty() && out.back() == '_') out.pop_back();

    if (out.empty()) throw std::runtime_error("Invalid name for include guard.");
    out += "_H";
    return out;
}

static void atomic_write_text(const fs::path& out_path, const std::string& content) {
    fs::create_directories(out_path.parent_path());

    fs::path tmp = out_path;
    tmp += ".tmp";

    {
        std::ofstream f(tmp, std::ios::binary);
        if (!f) throw std::runtime_error("Failed to open temp file for writing: " + tmp.string());
        f << content;
        if (!f) throw std::runtime_error("Failed while writing: " + tmp.string());
    }

    // replace atomically when possible
    std::error_code ec;
    fs::rename(tmp, out_path, ec);
    if (ec) {
        // Fallback: remove target and try again (best effort)
        fs::remove(out_path, ec);
        ec.clear();
        fs::rename(tmp, out_path, ec);
        if (ec) {
            fs::remove(tmp, ec);
            throw std::runtime_error("Failed to move temp file into place: " + ec.message());
        }
    }
}

static void print_usage() {
    std::cerr <<
R"(Usage:
  preprocess --config <json_config_flie>
)";
}

static std::string get_arg_value(int& i, int argc, char** argv) {
    if (i + 1 >= argc) throw std::runtime_error(std::string("Missing value for: ") + argv[i]);
    return std::string(argv[++i]);
}

int main(int argc, char** argv) {

    std::filesystem::path cfg_path = "config/config.json";

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--config" && i + 1 < argc) cfg_path = argv[++i];
        else if (a == "-h" || a == "--help") { print_usage(); return 0; }
    }

    auto cfg = config::load_config_json(cfg_path);

    Provenance prov;
    prov.generator_id   = "hls-preprocess-cpp";
    prov.source         = cfg_path.string();
    prov.git_sha        = get_git_sha(".");
    prov.build_time_iso = now_iso8601_utc();
    prov.config_path    = cfg_path.string();
    prov.config_sha256  = sha256_file(cfg_path.string());

    auto header_file_0_values = preprocess_hf0::generate(cfg.window_length, cfg.etc);
    // Call here additional preprocess and store their results in variables
    

    for (const auto& hs : cfg.headers) {
        Meta meta;
        meta.filename    = hs.file;
        meta.description = hs.description;
        meta.author      = cfg.meta.author;
        meta.version     = cfg.meta.version;
        meta.license     = cfg.meta.license;
        meta.copyright   = cfg.meta.copyright;

        const std::string date = today_yyyy_mm_dd();

        std::ostringstream out;
        Emitter emit(out);
        emit.set_float_format(9);

        emit.emit_provenance_block(prov);
        emit.emit_doxygen_file_block(meta, date);

        const std::string guard = make_include_guard(std::filesystem::path(hs.file).stem().string());
        emit.begin_guard(guard);
        emit.include_hls_defaults();

         emit.emit_constants(hs.ints, hs.floats);

        for (const auto& sec : hs.emit) {
            if (sec == "function_0") emit_section_function_0(emit, function_0_values);
            else if (sec == "function_1")  trig::emit_section_sin_cos(emit, trig);
        }

        emit.end_guard(guard);

        const std::filesystem::path out_path = cfg.output_dir / hs.file;
        atomic_write_text(out_path, out.str());
    }
}


