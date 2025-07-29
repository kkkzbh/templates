

template<typename T,typename Cmp = std::less<>>
struct dualheap
{
    /*
     * 对顶堆
     *  less    -> 第 k 大
     *  greater -> 第 k 小
     *  1-based
     */
    struct revcmp
    {
        auto operator()(T const& x,T const& y) const
        { return cmp(y,x); }
        Cmp cmp;
    };

    explicit constexpr dualheap(Cmp cmp = {},int kv = 1) : k(kv),comp(cmp),qx(revcmp{ cmp }),qy(std::move(cmp)) {}

    template<typename U>
    auto insert(U&& x)
    {
        if(qx.size() < k) {
            qx.push(std::forward<U>(x));
            return;
        }
        if(comp(x,qx.top())) {
            qy.push(std::forward<U>(x));
            return;
        }
        qx.push(std::forward<U>(x));
        qy.push(qx.top());
        qx.pop();
    }

    auto reindex(int kv) -> void
    {
        k = kv;
        auto need = k - int(qx.size());
        if(not need) {
            return;
        }
        if(need > 0) {
            do {
                qx.emplace(qy.top());
                qy.pop();
            } while(--need);
            return;
        }
        do {
            qy.emplace(qx.top());
            qx.pop();
        } while(++need);
    }

    auto top() -> T const&
    { return qx.top(); }

    auto size() -> std::size_t
    { return qx.size() + qy.size(); }

    int k;
    Cmp comp;
    std::priority_queue<T,std::vector<T>,revcmp> qx;
    std::priority_queue<T,std::vector<T>,Cmp> qy;
};