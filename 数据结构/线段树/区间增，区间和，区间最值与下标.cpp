// C++20

template<typename T = int>
struct segment
{
    /*
     * 如果有多个最大值，返回最左的
    */
    template<std::random_access_iterator It>
    auto build(int i,int l,int r,It first_) noexcept -> void
    {
        if(r - l == 1) {
            a[i] = first_[l];
            maxi[i] = l;
            maxv[i] = first_[l];
        } else {
            auto mid = std::midpoint(l,r);
            build(left(i),l,mid,first_);
            build(right(i),mid,r,first_);
            merge(i);
        }
    }

    explicit segment(std::integral auto n) noexcept : a(n * 4),lazy(n * 4),maxv(n * 4),maxi(n * 4)
    { build(0,0,n,a.begin()); }

    template<std::random_access_iterator It,typename Se>
    requires requires(It it) { { *it } -> std::convertible_to<T>; } and std::sentinel_for<Se,It>
    segment(It first_,Se end_) noexcept : segment(std::distance(first_,end_))
    { build(0,0,std::distance(first_,end_),first_); }

    template<std::ranges::random_access_range Range>
    explicit segment(Range&& r) noexcept : segment(std::ranges::begin(r),std::ranges::end(r)) {}

    [[nodiscard]]
    auto sum(int l,int r) noexcept -> T
    { return sum(0,0,(a.size() >> 2),l,r); }

    auto sum(int i,int l,int r,int lt,int rt) noexcept -> T
    {
        if(lt <= l and r <= rt) {
            return a[i];
        }
        auto mid = std::midpoint(l,r);
        update(i,mid - l,r - mid);
        if(lt >= mid) {
            return sum(right(i),mid,r,lt,rt);
        }
        if(rt <= mid) {
            return sum(left(i),l,mid,lt,rt);
        }
        auto constexpr fn = std::plus{};
        return fn(sum(left(i),l,mid,lt,rt),sum(right(i),mid,r,lt,rt));
    }

    auto add(int l,int r,T v) noexcept -> void
    { add(0,0,(a.size() >> 2),l,r,v); }

    auto add(int i,int l,int r,int lt,int rt,T v) -> void
    {
        if(l >= lt and r <= rt) {
            cum(i,r - l,v);
        } else {
            auto mid = std::midpoint(l,r);
            update(i,mid - l,r - mid);
            if(lt < mid) {
                add(left(i),l,mid,lt,rt,v);
            }
            if(rt > mid) {
                add(right(i),mid,r,lt,rt,v);
            }
            merge(i);
        }
    }

    auto max(int l,int r)
    {
        return [&,&seg = *this](this auto&& self,int i,int il,int ir) {
            if(l >= il and r <= ir) {
                return std::make_pair(seg.maxi[i],seg.maxv[i]);
            }
            auto mid = std::midpoint(il,ir);
            seg.update(i,mid - l,r - mid);
            if(l >= mid) {
                return self(right(i),mid,r);
            }
            if(r <= mid) {
                return self(left(i),l,mid);
            }
            auto [li,lv] = self(left(i),l,mid);
            auto [ri,rv] = self(right(i),mid,r);
            if(lv >= rv) {
                return std::make_pair(li,lv);
            }
            return std::make_pair(ri,rv);
        }(0,0,a.size() >> 2);;
    }

    [[nodiscard]]
    auto size() const noexcept -> int
    { return a.size() >> 2; }

    auto cum(int i,int n,T v) -> void
    {
        lazy[i] += v;
        maxv[i] += v;
        a[i] += n * v;
    }

    auto update(int i,int ln,int rn) noexcept -> void
    {
        if(lazy[i]) {
            cum(left(i),ln,lazy[i]);
            cum(right(i),rn,lazy[i]);
            lazy[i] = 0;
        }
    }

    auto merge(int i) noexcept -> void
    {
        a[i] = a[left(i)] + a[right(i)];
        maxv[i] = std::max(maxv[left(i)],maxv[right(i)]);
        maxi[i] = maxi[std::max(left(i),right(i),[&](int l,int r) -> bool {
            auto constexpr fn = std::less{};
            return fn(maxv[l],maxv[r]);
        })];
    }

    auto static constexpr left(int i) noexcept -> int
    { return (i << 1) | 1; }
    auto static constexpr right(int i) noexcept -> int
    { return left(i) + 1; }

    std::vector<T> a;
    std::vector<T> lazy;
    std::vector<T> maxv;
    std::vector<T> maxi;

};

template<std::random_access_iterator It,typename Se>
segment(It first_,Se end_) -> segment<typename std::iterator_traits<It>::value_type>;

template<std::ranges::random_access_range Range>
segment(Range&& r) -> segment<std::ranges::range_value_t<Range>>;