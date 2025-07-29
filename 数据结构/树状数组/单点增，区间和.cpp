

template<typename T = int>
struct fenwick
{
    template<std::integral I>
    explicit fenwick(I n) noexcept : a(std::vector<T>(n)) {}

    [[nodiscard]]
    auto size() const noexcept -> int
    { return a.size(); }

    auto add(int i,T v) noexcept
    {
        for(++i; i <= a.size(); i += i & -i) {
            a[i - 1] += v;
        }
    }

    [[nodiscard]]
    auto sum(int i) const noexcept -> T
    {
        T ret{};
        for(; i; i -= i & -i) {
            ret += a[i - 1];
        }
        return ret;
    }

    [[nodiscard]]
    auto sum(int l,int r) const noexcept -> T
    { return sum(r) - sum(l); }


    auto operator()(int l,int r) const noexcept -> T
    { return sum(l,r); }

    auto clear() noexcept
    { std::ranges::fill(a,T{}); }

    std::vector<T> a;
};