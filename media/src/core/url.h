#ifndef src_network_url_h_
#define src_network_url_h_

#include <string>

class Url {
public:
	explicit Url(const char* url) noexcept;
	explicit Url(const std::string& url) noexcept;

private:
	bool parse();
	bool parseProtocol(const char* url, size_t len, size_t& offset);
	bool parseHost(const char* url, size_t len, size_t& offset);
private:
	std::string m_url;

	std::string m_protocol;
	std::string m_host;
	int m_port{ 0 };
};

#endif // ! src_network_url_h_
