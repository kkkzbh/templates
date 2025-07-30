template<int N,int M,int id = 0>
struct trie
{
    using intx = int;

    auto insert(intx v) -> void
    {
        auto u = 1;
        ++pass[u];
        for(auto c = M; c--; ) {
            auto i = v >> c & 1;
            if(not a[u][i]) {
                a[u][i] = cnt++;
            }
            u = a[u][i];
            ++pass[u];
        }
        ++end[u];
    }

    auto count(intx v) const -> int
    {
        auto u = 1;
        for(auto c = M; c--; ) {
            auto i = v >> c & 1;
            if(not a[u][i]) {
                return 0;
            }
            u = a[u][i];
        }
        return end[u];
    }

    auto prefix_count(intx v) const -> int
    {
        auto u = 1;
        for(auto c = M; c--; ) {
            auto i = v >> c & 1;
            if(not a[u][i]) {
                return 0;
            }
            u = a[u][i];
        }
        return pass[u];
    }

    auto erase(intx v) -> void
    {
        if(not count(v)) {
            return;
        }
        auto u = 1;
        for(auto c = M; c--; ) {
            auto i = v >> c & 1;
            if(not --pass[a[u][i]]) {
                a[u][i] = 0;
                return;
            }
            u = a[u][i];
        }
        --end[u];
    }

    auto make_max(intx v) const -> intx
    {
        // 确保字典树中一定有一个数！
        auto u = 1;
        for(auto c = M; c--; ) {
            auto i = v >> c & 1;
            if(a[u][i ^ 1]) {
                v ^= intx{ i ^ 1 } << c;
                u = a[u][i ^ 1];
                continue;
            }
            v ^= intx{ i } << c;
            u = a[u][i];
        }
        return v;
    }

    ~trie()
    {
        for(auto i = 1; i != cnt; ++i) {
            a[i].fill(0);
        }
        std::fill(end.begin() + 1,end.begin() + cnt,0);
        std::fill(pass.begin() + 1,pass.begin() + cnt,0);
        cnt = first + 1;
    }

    auto constexpr static first = 1;
    int cnt = first + 1;
    std::array<std::array<int,2>,N> static inline a{};
    std::array<int,N> static inline end{};
    std::array<int,N> static inline pass{};
};