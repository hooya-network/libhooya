#include "hooya/pipeline/NetDataCache.hh"

namespace hooya::pipeline {
size_t NetDataCache::Add(
uint32_t key,
size_t offset,
const std::vector<uint8_t> &data
) {
	/* Get entry at key from cache, creating a new one if necessary */
	cache.insert({key, msg::NetData()});
	auto nh = cache.extract(key);

	/* Append incoming data to existing data */
	size_t ret = nh.mapped().AddAt(offset, data);
	cache.insert({key, nh.mapped()});

	return ret;
}

msg::NetData NetDataCache::Evict(uint32_t key) {
	auto node = cache.extract(key);
	if (!node)
		/* TODO Create an exception type for this case */
		throw "Refusing to evict from nonexistent key";

	return node.mapped();
} }
