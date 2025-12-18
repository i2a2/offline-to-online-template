#include "provenance.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#if __has_include(<openssl/sha.h>)
  #include <openssl/sha.h>
  #define HLS_PREPROC_HAVE_OPENSSL 1
#else
  #define HLS_PREPROC_HAVE_OPENSSL 0
#endif

//namespace preprocess {

static std::string rstrip_newlines(std::string s) {
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.pop_back();
    return s;
}

std::string now_iso8601_utc() {
    using clock = std::chrono::system_clock;
    const auto now = clock::now();
    const std::time_t t = clock::to_time_t(now);

    std::tm tm{};
#if defined(_WIN32)
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

std::string capture_command(const std::string& cmd) {
    // Use popen for simplicity; best-effort only.
    // Note: cmd should be controlled (no user injection).
    std::array<char, 256> buf{};
    std::string out;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return {};

    while (fgets(buf.data(), static_cast<int>(buf.size()), pipe) != nullptr) {
	out.append(buf.data());
    }
    const int rc = pclose(pipe);
    (void)rc;

    return rstrip_newlines(out);
}

static std::string git_cmd(const std::string& repo_root, const std::string& args) {
    // -C <repo_root> avoids depending on current working directory.
    std::ostringstream cmd;
    cmd << "git -C " << repo_root << " " << args << " 2>/dev/null";
    return capture_command(cmd.str());
}

std::string get_git_sha(const std::string& repo_root) {
    const std::string s = git_cmd(repo_root, "rev-parse HEAD");
    return s.empty() ? std::string("UNKNOWN") : s;
}

std::string sha256_file(const std::string& path) {
#if !HLS_PREPROC_HAVE_OPENSSL
    (void)path;
    return "UNKNOWN";
#else
    std::ifstream f(path, std::ios::binary);
    if (!f) return "UNKNOWN";

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    std::array<unsigned char, 4096> buf{};
    while (f.good()) {
	f.read(reinterpret_cast<char*>(buf.data()), static_cast<std::streamsize>(buf.size()));
	const std::streamsize n = f.gcount();
	if (n > 0) {
	    SHA256_Update(&ctx, buf.data(), static_cast<size_t>(n));
	}
    }

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_Final(digest, &ctx);

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (unsigned char b : digest) oss << std::setw(2) << static_cast<int>(b);
    return oss.str();
#endif
}
//}  // namespace preprocess
