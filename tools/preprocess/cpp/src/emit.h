#ifndef PREPROCESS_CPP_EMIT_H
#define PREPROCESS_CPP_EMIT_H

#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <ostream>
#include <string>

#include "provenance.hpp"

//struct Meta;

struct Meta {
    std::string filename;
    std::string description;
    std::string author;
    std::string version = "1.0";
    std::string copyright;
    std::string license;
};

class Emitter {

public:
    explicit Emitter(std::ostream& os);

    void emit_provenance_block(const Provenance& prov);
    void emit_doxygen_file_block(const Meta& meta, const std::string& date);

    void begin_guard(const std::string& guard);
    void end_guard(const std::string& guard);

    void include_hls_defaults();
    void emit_constants(const std::map<std::string, int64_t>& ints,
                        const std::map<std::string, double>& floats);

    void set_float_format(int decimals);

    void emit_array_1d_i32(const std::string& name, std::size_t n,
                           const std::function<int(std::size_t)>& at,
                           int per_line = 16);

    void emit_array_1d_f32(const std::string& name, std::size_t n,
                           const std::function<float(std::size_t)>& at,
                           int per_line = 8);

    // Scientific notation with 'f' suffix, e.g. 1.23456789f
    void emit_array_1d_f32_sci(const std::string& name, std::size_t n,
                               const std::function<float(std::size_t)>& at,
                               int per_line = 8);

    void emit_array_2d_f32(const std::string& name,
                           std::size_t d0, std::size_t d1,
                           const std::function<float(std::size_t,std::size_t)>& at);


    void emit_array_3d_f32(const std::string& name,
                           std::size_t d0, std::size_t d1, std::size_t d2,
                           const std::function<float(std::size_t,std::size_t,std::size_t)>& at);

    void emit_array_4d_f32(const std::string& name,
                           std::size_t d0, std::size_t d1, std::size_t d2, std::size_t d3,
                           const std::function<float(std::size_t,std::size_t,std::size_t,std::size_t)>& at);

    void emit_array_3d_i32(const std::string& name,
                           std::size_t d0, std::size_t d1, std::size_t d2,
                           const std::function<int(std::size_t,std::size_t,std::size_t)>& at);

    // Include array emitter as you need



private:
    std::ostream& os_;

    void indent(int n);

};


#endif //PREPROCESS_CPP_EMIT_H

