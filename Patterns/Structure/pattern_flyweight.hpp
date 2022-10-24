#pragma once

#include <string>
#include <vector>

namespace pattern_flyweight {

    struct CockroachContext {
        std::string name;
        int type;
        int x_pos;
    };

    struct CockroachFlyweight {
        CockroachFlyweight(int speed_, std::string property_) :
            speed(speed_), property(property_) {}
        int speed;
        std::string property;
        void display(CockroachContext& ctx) {
            ctx.x_pos += speed;
            int s = ctx.x_pos;
            while (s--) std::cout << " ";
            std::cout << ctx.name << property << std::endl;
        }
    };

    class CockroachFactory {
    public:
        CockroachFactory() = default;
        void AddCockroach(int speed, std::string property) {
            cache.push_back({speed, property});
        }
        int GetCount() { return cache.size(); }
        CockroachFlyweight* GetCockroach(int idx) { return &cache[idx]; }

    private:
        std::vector<CockroachFlyweight> cache;
    };
}
