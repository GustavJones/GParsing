#pragma once
#include "GParsing/HTTP/HTTPString.hpp"
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace GParsing {
	enum class HTTPMethod {
		Unknown = -1,
		Get,
		Head,
		Post
	};

	static const std::unordered_map<HTTPMethod, std::string> HTTPMethodStrings {
		{HTTPMethod::Unknown, ""},
		{HTTPMethod::Get, "GET"},
		{HTTPMethod::Head, "HEAD"},
		{HTTPMethod::Post, "POST"},
	};

	template<typename CharT>
	HTTPMethod HTTPGetMethod(const HTTPString<CharT> &_method) {
		for (const auto &methodString : HTTPMethodStrings) {
			if (methodString.second.length() != _method.Length()) {
				continue;
			}

			bool match = true;
			for (size_t i = 0; i < _method.Length(); i++) {
				if (_method[i] != methodString.second[i]) {
					match = false;
					break;
				}
			}

			if (match) {
				return methodString.first;
			}
		}

		return HTTPMethod::Unknown;
	}

	template<typename CharT>
	HTTPString<CharT> HTTPGetMethodString(const HTTPMethod _method) {
		if (_method == HTTPMethod::Unknown) throw std::runtime_error("Unknown method...");
		return HTTPMethodStrings.at(_method);
	}
}
