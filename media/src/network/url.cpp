#include "src/network/url.h"

static size_t firstNoSpace(const char* str, size_t len, size_t offset) {
	if (offset >= len) {
		return len;
	}
	const char* s = str;
	while (*s == ' ' && offset < len) {
		++s;
		++offset;
	}
	return offset;
}

static size_t find(const char* str, size_t len, size_t offset, char dest) {
	if (offset >= len) {
		return len;
	}

	const char* s = str;
	while (*s != dest && offset < len) {
		++s;
		++offset;
	}
	return offset;
}

Url::Url(const char* url) noexcept
	: Url(std::string(url)) {
}

Url::Url(const std::string& url) noexcept
	: m_url(url){
	parse();
	
}

bool Url::parse() {
	if (m_url.empty()) {
		return false;
	}
	// http://www.baidu.com
	const char* url = m_url.c_str();
	size_t len = m_url.size();
	size_t offset = firstNoSpace(url, len, 0);
	if (offset == len) {
		return false;
	}

	if (parseProtocol(url, len, offset)) {
		
	}

	return false;
}

bool Url::parseProtocol(const char* url, size_t len, size_t& offset) {
	if (offset >= len) {
		return false;
	}
	
	size_t pos = find(url, len, offset, ':');
	if (pos >= len) {
		offset = pos;
		return false;
	}

	m_protocol = m_url.substr(offset, pos - offset);
	offset = pos;
	return false;
}
