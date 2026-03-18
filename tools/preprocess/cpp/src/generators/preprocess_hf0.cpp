#include "preprocess_hf0.h"

/*
 * Compute here all values which can be preprocessed and expressed as tables before sintesis.
 * 
 */

preprocessor::Tables compute_tables(int window_length, int etc) {

    for (int i = 0; i < window_length; ++i) {
        // Compute values for values_0 and values_1 based on your requirements
        // For example:
        // t.values_0.push_back(some_function_of(i, etc));
        // t.values_1.push_back(another_function_of(i, etc));
    }

     preprocessor::Tables t;
     // Fill t.values_0 and t.values_1 with the computed values
     return t;

}

void preprocessor::emit_section_tables(Emitter& emit, const Tables& t) {

    emit.emit_array_1d_f32(
            "TABLE_VALUES_0",
            t.values_0.size(),
            [&](std::size_t i) -> float { return t.values_0[i]; },
            /*per_line=*/8
    );

    emit.emit_array_1d_f32(
            "TABLE_VALUES_1",
            t.values_1.size(),
            [&](std::size_t i) -> float { return t.values_1[i]; },
            /*per_line=*/8
    );
}

