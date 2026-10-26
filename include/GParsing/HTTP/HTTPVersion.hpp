#pragma once
#include "GParsing/HTTP/HTTPString.hpp"
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace GParsing {
	enum class HTTPVersion {
		Unknown,
		V10,
		V11,
		V20
	};

	static const std::unordered_map<HTTPVersion, std::string> HTTPVersionStrings {
		{HTTPVersion::Unknown, ""},
		{HTTPVersion::V10, "HTTP/1.0"},
		{HTTPVersion::V11, "HTTP/1.1"},
		{HTTPVersion::V20, "HTTP/2.0"},
	};

	template<typename CharT>
	HTTPVersion HTTPGetVersion(const HTTPString<CharT> &_version) {
		for (const auto &versionString : HTTPVersionStrings) {
			if (versionString.second.length() != _version.Length()) {
				continue;
			}

			bool match = true;
			for (size_t i = 0; i < _version.Length(); i++) {
				if (_version[i] != versionString.second[i]) {
					match = false;
					break;
				}
			}

			if (match) {
				return versionString.first;
			}
		}

		return HTTPVersion::Unknown;
	}

	template<typename CharT>
	HTTPString<CharT> HTTPGetVersionString(const HTTPVersion _version) {
		if (_version == HTTPVersion::Unknown) throw std::runtime_error("Unknown version...");
		return HTTPVersionStrings.at(_version);
	}
}
