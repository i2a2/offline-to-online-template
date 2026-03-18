#ifndef PREPROCESS_HF0_H
#define PREPROCESS_HF0_H

#pragma once
#include "../emit.h"

#include <cstddef>
#include <vector>

class preprocessor {
public:

    struct Tables {
        std::vector<float> values_0;
        std::vector<float> values_1;
    };

    static Tables compute_tables(int window_length, int etc);
    static void emit_section_tables(Emitter& emit, const Tables& t);
};

#endif //PREPROCESS_HF0_H

