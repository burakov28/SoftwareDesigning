#pragma once

#include <unordered_map>
#include <list>
#include <optional>
#include <vector>
#include <cassert>

template<typename Key, typename Value>
class LRUCache {
public:
  LRUCache(size_t lru_threshold = DEFAULT_LRU_THRESHOLD) : lru_threshold(lru_threshold) {
    if (lru_threshold == 0) {
      throw std::invalid_argument("Zero LRU threshold is prohibited");
    }
  }

  void set(const Key &key, const Value &value) {
    auto it = lru_cache.find(key);
    if (it != lru_cache.end()) {
      assert(erase(key));
    }
    if (lru_queue.size() == lru_threshold) {
      assert(erase(**(lru_queue.rbegin())));
    }

    auto pit = lru_cache.insert({key, {value, typename std::list<const Key *>::iterator()}});
    assert(pit.second);
    pit.first->second.second = lru_queue.insert(lru_queue.begin(), &pit.first->first);
  }

  bool erase(const Key &key) {
    auto it = lru_cache.find(key);
    if (it == lru_cache.end()) {
      return false;
    }

    lru_queue.erase(it->second.second);
    lru_cache.erase(it);
    return true;
  }

  std::optional<Value> get(const Key &key) {
    auto it = lru_cache.find(key);
    if (it == lru_cache.end()) {
      return std::optional<Value>();
    }
    const Key *key_ptr = *(it->second.second);

    lru_queue.erase(it->second.second);
    it->second.second = lru_queue.insert(lru_queue.begin(), key_ptr);
    return it->second.first;
  }

  std::vector<std::pair<Key, Value>> getLruOrder() const {
    std::vector<std::pair<Key, Value>> res;
    for (const Key *key_ptr : lru_queue) {
      auto it = lru_cache.find(*key_ptr);
      assert(it != lru_cache.end());
      res.push_back({*key_ptr, it->second.first});
    }
    return res;
  }

private:
  static constexpr size_t DEFAULT_LRU_THRESHOLD = 100;

  std::list<const Key *> lru_queue;
  std::unordered_map<Key, std::pair<Value, typename std::list<const Key *>::iterator>> lru_cache;
  size_t lru_threshold;
};
