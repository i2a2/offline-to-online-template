#include <cmath>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct Meta {
    std::string filename;
    std::string description = "<>";
    std::string author = "<>";
    std::string version = "1.0";
    std::string copyright = "<>";
    std::string license = "<>";
};

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

static std::string render_header(
    const std::string& name,
    const Meta& meta
) {
    const std::string guard = make_include_guard(name);
    const std::string date = today_yyyy_mm_dd();

    std::ostringstream oss;
    oss << "/******************************************************************************\n"
        << " * Filename    : " << meta.filename << "\n"
        << " * Description : " << meta.description << "\n"
        << " * Author      : " << meta.author << "\n"
        << " * Date        : " << date << "\n"
        << " * Version     : " << meta.version << "\n"
        << " *\n"
        << " * Copyright   : " << meta.copyright << "\n"
        << " * License     : " << meta.license << "\n"
        << " ******************************************************************************/\n\n";

    oss << "#ifndef " << guard << "\n"
        << "#define " << guard << "\n\n";

    // Includes
    //oss << "#include <hls_stream.h>\n"
    //    << "#include <hls_vector.h>\n"
    //    << "#include <hls_math.h>\n\n";

    return oss.str();
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
  preprocess trig --name <stem> --out <path> [options]

Command: trig
  --name         Base name for include guard (e.g., trig_tables)
  --out          Output header path (e.g., src/common/generated/trig_tables.h)
  --description  Header description
  --author       Author field
  --version      Version field (default: 1.0)
  --copyright    Copyright field
  --license      License field

Examples:
  preprocess trig --name <file_name> --out src/common/generated/<file_name>.h --author <author>
)";
}

static std::string get_arg_value(int& i, int argc, char** argv) {
    if (i + 1 >= argc) throw std::runtime_error(std::string("Missing value for: ") + argv[i]);
    return std::string(argv[++i]);
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            print_usage();
            return 2;
        }

        const std::string cmd = argv[1];
        if (cmd != "trig") {
            print_usage();
            return 2;
        }

        std::string name;
        fs::path out;
        Meta meta;

        for (int i = 2; i < argc; ++i) {
            std::string a = argv[i];

            if (a == "--name") name = get_arg_value(i, argc, argv);
            else if (a == "--out") out = fs::path(get_arg_value(i, argc, argv));
            else if (a == "--description") meta.description = get_arg_value(i, argc, argv);
            else if (a == "--author") meta.author = get_arg_value(i, argc, argv);
            else if (a == "--version") meta.version = get_arg_value(i, argc, argv);
            else if (a == "--copyright") meta.copyright = get_arg_value(i, argc, argv);
            else if (a == "--license") meta.license = get_arg_value(i, argc, argv);
            else if (a == "-h" || a == "--help") { print_usage(); return 0; }
            else throw std::runtime_error("Unknown argument: " + a);
        }

        if (name.empty()) throw std::runtime_error("--name is required");
        if (out.empty()) throw std::runtime_error("--out is required");

        meta.filename = out.filename().string();

        std::string hdr = render_header(name, meta);

        atomic_write_text(out, hdr);

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}

