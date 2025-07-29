// C++20

auto pow(i64 x,auto p) -> i64
{
    auto ans = 1;
    while(p) {
        if(p & 1) {
            ans *= x;
        }
        x *= x;
        p >>= 1;
    }
    return ans;
}

auto pow(i64 x,auto p,auto mod) -> i64 
{
    auto ans = 1;
    while(p) {
        if(p & 1) {
            ans *= x;
            ans %= mod;
        }
        x *= x;
        x %= mod;
        p >>= 1;
    }
    return ans;
}