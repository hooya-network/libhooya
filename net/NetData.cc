#include "hooya/net/NetData.hh"

namespace hooya::msg {
size_t NetData::AddAt(size_t at, const std::vector<uint8_t> &inData) {
	size_t to = inData.size() + at;

	/* Append or overwrite data depending on `at` relative to existing data */
	if (at > data.size()) {
		data.resize(at - 1);
		data.insert(data.end(), inData.begin(), inData.end());
	} else {
		if (to > data.size())
			data.resize(to);
		std::copy(inData.begin(), inData.end(), data.begin() + at);
	}

	return data.size();
}

size_t NetData::Size() const {
	return data.size();
}

const std::vector<uint8_t> &NetData::Raw() {
	return data;
} }
