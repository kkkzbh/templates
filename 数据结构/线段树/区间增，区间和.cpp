

template<typename T = int>
struct segment
{

    auto build(int i,int l,int r,auto const& vec) -> void
    {
        if(r - l == 1) {
            a[i] = vec[l];
            return;
        }
        auto mid = (l + r) / 2;
        build(left(i),l,mid,vec);
        build(right(i),mid,r,vec);
        merge(i);
    }

    explicit segment(size_t n) : a(4 * n),lz(4 * n) {}

    explicit segment(auto const& vec)
    : segment(vec.size())
    { build(0,0,vec.size(),vec); }

    auto merge(int i) -> void
    { a[i] = a[left(i)] + a[right(i)]; }

    auto update(int i,int il,int ir) -> void
    {
        if(lz[i]) {
            auto li = left(i),ri = right(i);
            lz[li] += lz[i];
            lz[ri] += lz[i];
            auto mid = (il + ir) / 2;
            a[li] += (mid - il) * lz[i];
            a[ri] += (ir - mid) * lz[i];
            lz[i] = 0;
        }
    }

    auto add(int l,int r,auto val) -> void
    { add(0,0,a.size() / 4,l,r,val); }

    auto add(int i,int il,int ir,int l,int r,auto val) -> void
    {
        if(il >= l and ir <= r) {
            a[i] += 1LL * (ir - il) * val;
            lz[i] += val;
            return;
        }
        update(i,il,ir);
        auto mid = (il + ir) / 2;
        if(l < mid) {
            add(left(i),il,mid,l,r,val);
        }
        if(r > mid) {
            add(right(i),mid,ir,l,r,val);
        }
        merge(i);
    }

    auto sum(int l,int r) -> i64
    { return sum(0,0,a.size() / 4,l,r); }

    auto sum(int i,int il,int ir,int l,int r) -> i64
    {
        if(il >= l and ir <= r) {
            return a[i];
        }
        update(i,il,ir);
        auto mid = (il + ir) / 2;
        auto ret = 0LL;
        if(l < mid) {
            ret += sum(left(i),il,mid,l,r);
        }
        if(r > mid) {
            ret += sum(right(i),mid,ir,l,r);
        }
        return ret;
    }

    auto static left(int i) -> int
    { return i << 1 | 1; }

    auto static right(int i) -> int
    { return left(i) + 1; }


    std::vector<T> a,lz;
};