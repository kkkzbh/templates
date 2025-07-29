

auto eratosthenes(int n) -> std::vector<bool>
{
    auto pv = std::vector(n + 1,true);
    pv[0] = pv[1] = false;
    for(auto i = 3; i <= n; i += 2) {
        if(not pv[i]) {
            continue;
        }
        for(auto j = 1ull * i * i; j <= n; j += i) {
            pv[j] = false;
        }
    }
    return pv;
}
