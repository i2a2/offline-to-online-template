#include "emit.h"
#include "provenance.hpp" // struct Provenance
#include <algorithm>
#include <iomanip>
#include <vector>
#include <limits>

Emitter::Emitter(std::ostream& os) : os_(os) {}

void Emitter::indent(int n) {
    for (int i = 0; i < n; ++i) os_ << ' ';
}

void Emitter::set_float_format(int decimals) {
    os_ << std::fixed << std::setprecision(decimals);
}

void Emitter::emit_provenance_block(const Provenance& prov) {
    // Doxygen-friendly: @note + @par
    os_ << "/**\n"
        << " * @note AUTO-GENERATED FILE. DO NOT EDIT.\n"
        << " *\n"
        << " * @par Provenance\n"
        << " * Generator ID: " << (prov.generator_id.empty() ? "UNKNOWN" : prov.generator_id) << "\\n\n"
        << " * Source: "       << (prov.source.empty()       ? "UNKNOWN" : prov.source)       << "\\n\n"
        << " * Git commit: "   << (prov.git_sha.empty()      ? "UNKNOWN" : prov.git_sha)      << "\\n\n"
        << " * Build time: "   << (prov.build_time_iso.empty()? "UNKNOWN": prov.build_time_iso) << "\\n\n";

    if (prov.config_path) {
        os_ << " * Config path: " << *prov.config_path << "\\n\n";
    }
    if (prov.config_sha256) {
        os_ << " * Config SHA256: " << *prov.config_sha256 << "\\n\n";
    }

    auto print_kv = [&](const char* title, const auto& vec) {
        if (vec.empty()) return;
        std::vector<std::pair<std::string,std::string>> tmp(vec.begin(), vec.end());
        std::sort(tmp.begin(), tmp.end(), [](auto& a, auto& b){ return a.first < b.first; });

        os_ << " * " << title << ":\n";
        for (const auto& [k,v] : tmp) {
            os_ << " *   - " << k << ": " << v << "\n";
        }
        os_ << " *\n";
    };

    print_kv("Parameters", prov.params);
    print_kv("Inputs", prov.inputs);

    os_ << " */\n\n";
}

void Emitter::emit_doxygen_file_block(const Meta& meta, const std::string& date) {
    os_ << "/**\n"
        << " * @file    " << meta.filename << "\n"
        << " * @brief   " << meta.description << "\n"
        << " * @author  " << meta.author << "\n"
        << " * @date    " << date << "\n"
        << " * @version " << meta.version << "\n"
        << " *\n"
        << " * @copyright " << meta.copyright << "\n"
        << " * @license   " << meta.license << "\n"
        << " */\n\n";
}

void Emitter::begin_guard(const std::string& guard) {
    os_ << "#ifndef " << guard << "\n"
        << "#define " << guard << "\n\n";
}

void Emitter::end_guard(const std::string& guard) {
    os_ << "\n#endif // " << guard << "\n";
}

void Emitter::include_hls_defaults() {
    os_ << "#include <hls_stream.h>\n"
        << "#include <hls_vector.h>\n"
        << "#include <hls_math.h>\n\n";
}

void Emitter::emit_constants(const std::map<std::string, int64_t>& ints,
                             const std::map<std::string, double>& floats) {
    // Ordenados por clave al ser map
    for (const auto& [k,v] : ints) {
        os_ << "static const int " << k << " = " << v << ";\n";
    }
    for (const auto& [k,v] : floats) {
        // Mantener fixed + precision actual (recomendación: set_float_format(9) antes)
        os_ << "static const float " << k << " = " << v << ";\n";
    }
    if (!ints.empty() || !floats.empty()) os_ << "\n";
}


void Emitter::emit_array_1d_i32(const std::string &name, std::size_t n, const std::function<int(std::size_t)> &at,
                                int per_line) {
    os_ << "static const int " << name << "[" << n << "] = {\n";
    for (std::size_t i = 0; i < n; ++i) {
        if (i % (std::size_t)per_line == 0) indent(4);
        os_ << at(i);
        if (i + 1 != n) os_ << ", ";
        if ((i + 1) % (std::size_t)per_line == 0) os_ << "\n";
    }
    if (n % (std::size_t)per_line != 0) os_ << "\n";
    os_ << "};\n\n";
}

void Emitter::emit_array_1d_f32(const std::string& name, std::size_t n,
                                const std::function<float(std::size_t)>& at,
                                int per_line) {
    os_ << "static const float " << name << "[" << n << "] = {\n";
    for (std::size_t i = 0; i < n; ++i) {
        if (i % (std::size_t)per_line == 0) indent(4);
        os_ << at(i);
        if (i + 1 != n) os_ << ", ";
        if ((i + 1) % (std::size_t)per_line == 0) os_ << "\n";
    }
    if (n % (std::size_t)per_line != 0) os_ << "\n";
    os_ << "};\n\n";
}


void Emitter::emit_array_1d_f32_sci(const std::string &name, std::size_t n, const std::function<float(std::size_t)> &at,
                                    int per_line) {

    const auto old_flags = os_.flags();
    const auto old_prec  = os_.precision();
    const auto old_loc   = os_.getloc();

    os_.imbue(std::locale::classic());
    os_ << std::scientific
        << std::setprecision(std::numeric_limits<float>::max_digits10);

    os_ << "static const float " << name << "[" << n << "] = {\n";
    for (std::size_t i = 0; i < n; ++i) {
        if (i % (std::size_t)per_line == 0) indent(4);
        os_ << static_cast<double>(at(i));
        if (i + 1 != n) os_ << ", ";
        if ((i + 1) % (std::size_t)per_line == 0) os_ << "\n";
    }
    if (n % (std::size_t)per_line != 0) os_ << "\n";
    os_ << "};\n\n";

    os_.flags(old_flags);
    os_.precision(old_prec);
    os_.imbue(old_loc);
}

void Emitter::emit_array_2d_f32(const std::string &name, std::size_t d0, std::size_t d1,
                                const std::function<float(std::size_t, std::size_t)> &at) {

    const auto old_flags = os_.flags();
    const auto old_prec  = os_.precision();
    const auto old_loc   = os_.getloc();

    os_.imbue(std::locale::classic());
    os_ << std::scientific
        << std::setprecision(std::numeric_limits<float>::max_digits10);

    os_ << "static const float " << name
        << "[" << d0 << "][" << d1 << "] = {\n";

    for (std::size_t i = 0; i < d0; ++i) {
        indent(2); os_ << "{";
        for (std::size_t j = 0; j < d1; ++j) {
            os_ << static_cast<double>(at(i, j));
            if (j + 1 != d1) os_ << ", ";
        }
        os_ << "}";
        if (i + 1 != d0) os_ << ",\n";
        else os_ << "\n";
    }
    os_ << "};\n\n";

    os_.flags(old_flags);
    os_.precision(old_prec);
    os_.imbue(old_loc);
}

void Emitter::emit_array_3d_i32(const std::string& name,
                                std::size_t d0, std::size_t d1, std::size_t d2,
                                const std::function<int(std::size_t,std::size_t,std::size_t)>& at) {
    os_ << "static const int " << name
        << "[" << d0 << "][" << d1 << "][" << d2 << "] = {\n";

    for (std::size_t i = 0; i < d0; ++i) {
        indent(2); os_ << "{\n";
        for (std::size_t j = 0; j < d1; ++j) {
            indent(4); os_ << "{";
            for (std::size_t k = 0; k < d2; ++k) {
                os_ << at(i,j,k);
                if (k + 1 != d2) os_ << ", ";
            }
            os_ << "}";
            if (j + 1 != d1) os_ << ",\n";
            else os_ << "\n";
        }
        indent(2); os_ << "}";
        if (i + 1 != d0) os_ << ",\n";
        else os_ << "\n";
    }
    os_ << "};\n\n";
}

void Emitter::emit_array_3d_f32(const std::string& name,
                                std::size_t d0, std::size_t d1, std::size_t d2,
                                const std::function<float(std::size_t,std::size_t,std::size_t)>& at) {
    os_ << "static const float " << name
        << "[" << d0 << "][" << d1 << "][" << d2 << "] = {\n";

    for (std::size_t i = 0; i < d0; ++i) {
        indent(2); os_ << "{\n";
        for (std::size_t j = 0; j < d1; ++j) {
            indent(4); os_ << "{";
            for (std::size_t k = 0; k < d2; ++k) {
                os_ << at(i,j,k);
                if (k + 1 != d2) os_ << ", ";
            }
            os_ << "}";
            if (j + 1 != d1) os_ << ",\n";
            else os_ << "\n";
        }
        indent(2); os_ << "}";
        if (i + 1 != d0) os_ << ",\n";
        else os_ << "\n";
    }
    os_ << "};\n\n";
}

void Emitter::emit_array_4d_f32(const std::string& name,
                                std::size_t d0, std::size_t d1, std::size_t d2, std::size_t d3,
                                const std::function<float(std::size_t,std::size_t,std::size_t,std::size_t)>& at) {
    os_ << "static const float " << name
        << "[" << d0 << "][" << d1 << "][" << d2 << "][" << d3 << "] = {\n";

    for (std::size_t i = 0; i < d0; ++i) {
        indent(2); os_ << "{\n";
        for (std::size_t j = 0; j < d1; ++j) {
            indent(4); os_ << "{\n";
            for (std::size_t k = 0; k < d2; ++k) {
                indent(6); os_ << "{";
                for (std::size_t n = 0; n < d3; ++n) {
                    os_ << at(i,j,k,n);
                    if (n + 1 != d3) os_ << ", ";
                }
                os_ << "}";
                if (k + 1 != d2) os_ << ",\n";
                else os_ << "\n";
            }
            indent(4); os_ << "}";
            if (j + 1 != d1) os_ << ",\n";
            else os_ << "\n";
        }
        indent(2); os_ << "}";
        if (i + 1 != d0) os_ << ",\n";
        else os_ << "\n";
    }

    os_ << "};\n\n";
}


