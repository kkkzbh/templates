// C++20

auto constexpr dynamic_mod = 0;
 
template<typename T>
concept integer = std::integral<T> or std::same_as<T, __int128>;
 
template<integer auto MOD, typename MT = decltype(MOD)>
struct basic_mod
{
    auto constexpr static mod = MOD;
};
 
template<typename MT>
struct basic_mod<dynamic_mod, MT>
{
    MT mod;
};
 
template<integer T, integer auto MOD = dynamic_mod, integer MT = decltype(MOD)>
struct basic_mint : basic_mod<MOD, MT>
{
    using super = basic_mod<MOD, MT>;
    using difference_type = std::ptrdiff_t;
 
    #define M_DYNAMIC_MOD requires (MOD == dynamic_mod)
    #define M_NOT_DYNAMIC_MOD requires (not (MOD == dynamic_mod))
 
    constexpr basic_mint() M_NOT_DYNAMIC_MOD = default;
 
    constexpr basic_mint(integer auto v) M_NOT_DYNAMIC_MOD : val(v % this->mod)
    {
        norm<false>();
    }
 
    constexpr basic_mint(integer auto v,integer auto) M_NOT_DYNAMIC_MOD : basic_mint(v) {}
 
    constexpr basic_mint(basic_mint const&) = default;
 
    explicit constexpr basic_mint(integer auto mv) M_DYNAMIC_MOD : super(mv) {}
 
    constexpr basic_mint(integer auto v, integer auto mv) M_DYNAMIC_MOD : super(mv), val(v % this->mod)
    {
        norm<false>();
    }
 
    auto constexpr norm() noexcept -> void
    {
        if(val >= this->mod) {
            val -= this->mod;
        } else if(val < 0) {
            val += this->mod;
        }
    }
 
    template<bool big>
    auto constexpr norm() noexcept -> void
    {
        if constexpr(big) {
            if(val >= this->mod) {
                val -= this->mod;
            }
        } else {
            if(val < 0) {
                val += this->mod;
            }
        }
    }
 
    [[nodiscard]]
    auto constexpr inv() const -> basic_mint
    {
        return pow(this->mod - 2);
    }
 
    [[nodiscard]]
    auto constexpr pow(integer auto p) const -> basic_mint
    {
        basic_mint v{1, this->mod}, mv{*this};
        for(; p; p >>= 1) {
            if(p & 1) {
                v *= mv;
            }
            mv *= mv;
        }
        return v;
    }
 
    auto constexpr operator=(integer auto v) -> basic_mint &
    {
        val = v % this->mod;
        norm<false>();
        return *this;
    }
 
    auto constexpr operator=(basic_mint const&) -> basic_mint& = default;
 
    auto constexpr operator^(integer auto p) const -> basic_mint { return pow(p); }
 
    constexpr operator T() const noexcept
    {
        return val;
    }
 
    constexpr explicit operator bool() const noexcept
    {
        return bool(val);
    }
 
    auto constexpr operator-() const noexcept -> basic_mint
    {
        basic_mint ret;
        ret.val = this->mod - val;
        if constexpr(MOD == dynamic_mod) {
            ret.mod = this->mod;
        }
        return ret;
    }
 
    auto constexpr operator+=(basic_mint v) -> basic_mint &
    {
        val += v.val;
        norm<true>();
        return *this;
    }
 
    auto constexpr operator+=(std::integral auto v) -> basic_mint &
    {
        val += v;
        val %= this->mod;
        norm();
        return *this;
    }
 
    auto constexpr operator*=(basic_mint v) -> basic_mint &
    {
        val = static_cast<unsigned long long>(val) * v.val % this->mod;
        return *this;
    }
 
    auto constexpr operator*=(std::integral auto v) -> basic_mint &
    {
        if constexpr(std::signed_integral<decltype(v)>) {
            if(v < 0) {
                v %= this->mod;
                v += this->mod;
            }
        }
        val = static_cast<unsigned long long>(val) * v % this->mod;
        return *this;
    }
 
    auto constexpr operator-=(basic_mint v) -> basic_mint &
    {
        val -= v.val;
        norm<false>();
        return *this;
    }
 
    auto constexpr operator-=(std::integral auto v) -> basic_mint &
    {
        val -= v;
        norm();
        return *this;
    }
 
    auto constexpr operator/=(basic_mint v) -> basic_mint &
    {
        *this *= v.inv();
        return *this;
    }
 
    auto constexpr operator/=(std::integral auto v) -> basic_mint &
    {
        *this *= basic_mint{v, this->mod}.inv();
        return *this;
    }
 
    auto friend constexpr operator+(basic_mint x, basic_mint y) -> basic_mint
    {
        x += y;
        return x;
    }
 
    auto friend constexpr operator+(basic_mint x, std::integral auto y) -> basic_mint
    {
        x += y;
        return x;
    }
 
    auto friend constexpr operator+(std::integral auto x, basic_mint y) -> basic_mint
    {
        y += x;
        return y;
    }
 
    auto friend constexpr operator*(basic_mint x, basic_mint y) -> basic_mint
    {
        x *= y;
        return x;
    }
 
    auto friend constexpr operator*(basic_mint x, std::integral auto y) -> basic_mint
    {
        x *= y;
        return x;
    }
 
    auto friend constexpr operator*(std::integral auto x, basic_mint y) -> basic_mint
    {
        y *= x;
        return y;
    }
 
    auto friend constexpr operator-(basic_mint x, basic_mint y) -> basic_mint
    {
        x -= y;
        return x;
    }
 
    auto friend constexpr operator-(basic_mint x, std::integral auto y) -> basic_mint
    {
        x -= y;
        return x;
    }
 
    auto friend constexpr operator-(std::integral auto x, basic_mint y) -> basic_mint
    {
        y -= x;
        y = -y;
        return y;
    }
 
    auto friend constexpr operator/(basic_mint x, basic_mint y) -> basic_mint
    {
        x /= y;
        return x;
    }
 
    auto friend constexpr operator/(basic_mint x, std::integral auto y) -> basic_mint
    {
        x /= y;
        return x;
    }
 
    auto friend constexpr operator/(std::integral auto x, basic_mint y) -> basic_mint
    {
        auto xx = basic_mint{x, y.mod};
        xx /= y;
        return xx;
    }
 
    auto friend operator>>(auto &is, basic_mint &v) -> auto &
    {
        is >> v.val;
        v.val %= v.mod;
        v.norm();
        return is;
    }
 
    auto friend operator<<(auto &os, basic_mint v) -> auto & { return os << v.val; }
 
    auto friend constexpr operator==(basic_mint x, basic_mint y) -> bool { return x.val == y.val; }
 
    auto friend constexpr operator<=>(basic_mint x, basic_mint y)
    {
        return x.val <=> y.val;
    }
 
    auto operator++() -> basic_mint&
    {
        *this += 1;
        return *this;
    }
 
    auto operator++(int) -> T
    {
        auto ret = val;
        ++*this;
        return ret;
    }
 
    T val;
 
    #undef M_DYNAMIC_MOD
    #undef M_NOT_DYNAMIC_MOD
};
 
template<integer auto MOD>
using mint = basic_mint<int,MOD>;
 
using smint = mint<1000000000 + 7>;
 
using dmint = mint<dynamic_mod>;
 
namespace mint_literals
{
    auto constexpr operator""mi(unsigned long long v) -> smint
    { return v; }
}
 
using namespace mint_literals;