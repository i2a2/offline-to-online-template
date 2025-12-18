#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

//namespace preprocess {
struct Provenance {
    std::string generator_id;
    std::string source;
    std::string git_sha;
    std::string build_time_iso;

    std::vector<std::pair<std::string, std::string>> params;  // (key, value)
    std::optional<std::string> config_path;
    std::optional<std::string> config_sha256;
    std::vector<std::pair<std::string, std::string>> inputs;  // (key, value)
};

// ---- Provenance collection helpers ----

std::string now_iso8601_utc();

// Run a shell command and capture stdout (trimmed). Returns empty on failure.
std::string capture_command(const std::string& cmd);

// Best-effort git metadata; returns "UNKNOWN" if unavailable.
std::string get_git_sha(const std::string& repo_root = ".");

// SHA256 helpers (config hashing). Returns "UNKNOWN" if not supported / file missing.
std::string sha256_file(const std::string& path);

//}  // namespace preprocess
