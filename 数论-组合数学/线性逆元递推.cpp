

auto invs(int n,int p) -> std::vector<int>
{
    auto pv = std::vector(n + 1,0);
    pv[1] = 1;
    for(auto i = 2; i != n + 1; ++i) {
        pv[i] = p - 1LL * pv[p % i] + (p / i) % p;
    }
    return pv;
}