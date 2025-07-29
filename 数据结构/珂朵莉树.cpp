// C++20

template<typename F>
struct make_transparent : F
{
    using F::operator();
    using is_transparent = void;
    constexpr explicit make_transparent(F&& f) : F(std::forward<F>(f)) {}
    constexpr make_transparent() = default;
};
 
template<typename T = int>
struct chtholly
{
 
    explicit chtholly(int n,T v = 0) : set({ std::make_pair(std::make_pair(0,n),v) }) {}
 
    template<typename R>
    explicit chtholly(R&& r)
    {
        auto lasti = 0,lastv = *r.begin();
        for(auto [i,v] : zip(iota(1),r | drop(1))) {
            if(v == lastv) {
                continue;
            }
            set.emplace(std::piecewise_construct,std::forward_as_tuple(lasti,i),std::forward_as_tuple(lastv));
            lasti = i;
            lastv = v;
        }
        set.emplace(std::piecewise_construct,std::forward_as_tuple(lasti,r.size()),std::forward_as_tuple(lastv));
    }
 
    auto split(int p)
    {
        auto it = set.lower_bound(p);
        auto [pv,v] = *it;
        auto [l,r] = pv;
        if(r == p) {
            return ++it;
        }
        set.erase(it);
        set.emplace(std::piecewise_construct,std::forward_as_tuple(l,p),std::forward_as_tuple(v));
        return std::get<0>(set.emplace(std::piecewise_construct,std::forward_as_tuple(p,r),std::forward_as_tuple(v)));
    }
 
    auto split(int l,int r)
    {
        auto ir = split(r),il = split(l);
        return std::make_pair(il,ir);
    }
 
    auto modify(int l,int r,T v) -> void
    {
        " [l,r) 修改为 v "; // NOLINT
        auto [il,ir] = split(l,r);
        set.erase(il,ir);
        set.emplace(std::piecewise_construct,std::forward_as_tuple(l,r),std::forward_as_tuple(v));
    }
 
    auto add(int l,int r,T v) -> void
    {
        " [l,r) 增加v "; // NOLINT
        auto [il,ir] = split(l,r);
        for(auto& val : subrange(il,ir) | values) {
            val += v;
        }
    }
 
    auto kth(int l,int r,int k) -> T
    {
        " [l,r) 第k小的数 (排序后第k个数) "; // NOLINT
        auto [il,ir] = split(l,r);
        auto a = subrange(il,ir) | transform([](auto const& p) {
            auto const& [pv,v] = p;
            auto [l,r] = pv;
            return std::make_pair(r - l,v);
        }) | to<std::vector>();
        std::ranges::sort(a,{},[](auto const& p) { return std::get<1>(p); });
        for(auto [cnt,v] : a) {
            if(k <= cnt) {
                return v;
            }
            k -= cnt;
        }
        " 一般调用者应该保证 k 不超出(r - l) "; // NOLINT
        return -1;
    }
 
    auto sump(int l,int r,int x,auto y) -> decltype(y)
    {
        " 范围[l,r)内的数 a^x mod y "; // NOLINT
        auto ans = 0;
        auto [il,ir] = split(l,r);
        for(auto const& [pv,v] : subrange(il,ir)) {
            auto [l,r] = pv;
            ans += (r - l) * pow(v,x,y); // 要求使用乘法快速幂 取模版本
            ans %= y;
        }
        return ans;
    }
 
    using node = std::pair<int,int>;
 
    auto static constexpr cmp = make_transparent {
        []<typename CT,typename CU>(CT const& px,CU const& py) {
            auto constexpr fn = std::less{};
            auto constexpr index = 1;
            if constexpr(std::same_as<CT,node>) {
                if constexpr(std::same_as<CU,node>) {
                    return fn(std::get<index>(px),std::get<index>(py));
                } else {
                    return fn(std::get<index>(px),py);
                }
            } else {
                return fn(px,std::get<index>(py));
            }
        }
    };
 
    std::map<node,T,decltype(cmp)> set;
};
 
template<typename R>
chtholly(R&& r) -> chtholly<std::ranges::range_value_t<R>>;