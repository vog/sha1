#include "sha1.hpp"

class SHA1Top : public SHA1 {
public:
	std::string hash(const std::string &message)
	{
		this->update(message);
		return this->final();
	}
};