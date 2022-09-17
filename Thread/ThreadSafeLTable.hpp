#pragma once

#include <shared_mutex>
#include <list>
#include <vector>
#include <utility>
#include <algorithm>

namespace ThreadSafeLTable {

    template <typename Key, typename Value, typename Hash = std::hash<Key>>
    class ThreadSafeLTable {
    private:
        class bucket_type {
            typedef std::pair<Key, Value> bucket_value;
            typedef std::list<bucket_value> bucket_data;
            typedef typename bucket_data::iterator bucket_iterator;
            typedef typename bucket_data::const_iterator bucket_iterator_const;

            bucket_data data;
            mutable std::shared_mutex mutex;

            bucket_iterator_const find_entry_for(Key const& key) const {
                return std::find_if(data.begin(), data.end(), [&](bucket_value const& item) {
                    return item.first == key;
                });
            }

            bucket_iterator find_entry_for(Key const& key) {
                return std::find_if(data.begin(), data.end(), [&](bucket_value const& item) {
                    return item.first == key;
                });
            }

        public:
            bool getValue(Key const& key, Value& value) const {
                std::shared_lock lock(mutex);
                bucket_iterator_const found_entry = find_entry_for(key);
                if (found_entry == data.end())
                    return false;
                value = found_entry->second;
                return true;
            }

            bool addValue(Key const& key, Value const& value) {
                std::lock_guard lock(mutex);
                bucket_iterator found_entry = find_entry_for(key);
                if (found_entry == data.end()) {
                    data.push_back(bucket_value(key, value));
                    return true;
                }
                found_entry->second = value;
                return false;
            }

            bool removeValue(Key const& key) {
                std::lock_guard lock(mutex);
                const bucket_iterator found_entry = find_entry_for(key);
                if (found_entry != data.end()) {
                    data.erase(bucket_iterator);
                    return true;
                }
                return false;
            }
        };

        std::vector<std::unique_ptr<bucket_type>> bucklets;
        Hash hasher;

        bucket_type& get_bucklet(Key const& key) const {
            std::size_t const index = hasher(key) % bucklets.size();
            return *bucklets[index];
        }

    public:
        ThreadSafeLTable(unsigned num_bucklets = 20, Hash const& hasher_ = Hash()) :
            hasher(hasher_) {
            while (num_bucklets--)
                bucklets.push_back(std::make_unique<bucket_type>());
        }
        ThreadSafeLTable(const ThreadSafeLTable&) = delete;
        ThreadSafeLTable& operator=(const ThreadSafeLTable&) = delete;

        bool getValue(Key const& key, Value& value) const {
            return get_bucklet(key).getValue(key, value);
        }

        bool addValue(Key const& key, Value const& value) const {
            return get_bucklet(key).addValue(key, value);
        }

        bool removeValue(Key const& key) {
            return get_bucklet(key).removeValue(key);
        }
    };
}