template<int N,int id = 0>
struct trie
{

    auto constexpr static M = 2 * 26 + 10;
    auto static map(char c) -> int
    {
        if(std::islower(c)) {
            return c - 'a';
        }
        if(std::isupper(c)) {
            return 26 + c - 'A';
        }
        return 2 * 26 + c - '0';
    }

    auto insert(std::string_view str) -> void
    {
        auto u = 1;
        ++pass[u];
        for(auto c : str) {
            auto i = map(c);
            if(not a[u][i]) {
                a[u][i] = cnt++;
            }
            u = a[u][i];
            ++pass[u];
        }
        ++end[u];
    }

    auto count(std::string_view str) const -> int
    {
        auto u = 1;
        for(auto c : str) {
            auto i = map(c);
            if(not a[u][i]) {
                return 0;
            }
            u = a[u][i];
        }
        return end[u];
    }

    auto prefix_count(std::string_view str) const -> int
    {
        auto u = 1;
        for(auto c : str) {
            auto i = map(c);
            if(not a[u][i]) {
                return 0;
            }
            u = a[u][i];
        }
        return pass[u];
    }

    auto erase(std::string_view str) -> void
    {
        if(not count(str)) {
            return;
        }
        auto u = 1;
        for(auto c : str) {
            auto i = map(c);
            if(not --pass[a[u][i]]) {
                a[u][i] = 0;
                return;
            }
            u = a[u][i];
        }
        --end[u];
    }

    ~trie()
    {
        std::fill(a.begin() + 1,a.begin() + cnt,std::array<int,M>{});
        std::fill(end.begin() + 1,end.begin() + cnt,0);
        std::fill(pass.begin() + 1,pass.begin() + cnt,0);
        cnt = first + 1;
    }

    auto constexpr static first = 1;
    int cnt = first + 1;
    std::array<std::array<int,M>,N> static inline a{};
    std::array<int,N> static inline end{};
    std::array<int,N> static inline pass{};
};