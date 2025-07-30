struct disjoint_set
{

    explicit disjoint_set(int n) : n(n),a(n,-1) {}

    auto find(int i) -> int
    {
        if(a[i] < 0) {
            return i;
        }
        return a[i] = find(a[i]);
    }

    auto merge(int x,int y) -> bool
    {
        auto fx = find(x),fy = find(y);
        if(fx == fy) {
            return false;
        }
        if(a[fy] < a[fx]) {
            std::swap(fx,fy);
        }
        a[fx] += a[fy];
        a[fy] = fx;
        --n;
        return true;
    }

    auto same(int x,int y) -> bool
    {
        return find(x) == find(y);
    }

    auto size(int i) -> int
    {
        return -a[find(i)];
    }

    int n;
    std::vector<int> a;
};