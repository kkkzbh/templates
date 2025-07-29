auto constexpr default_sep = " "sv;
auto constexpr default_end = "\n"sv;
 
template<int Id>
struct parser
{
    constexpr parser(std::string_view s) : sv{s} {}
 
    auto friend operator<<(std::ostream &os, parser &pa) -> std::ostream &
    {
        os << pa.sv;
        return os;
    }
 
    auto constexpr clear() -> void
    {
        if constexpr(Id == 0) {
            sv = default_sep;
        } else {
            sv = default_end;
        }
    }
 
    auto operator=(this auto &self, std::string_view s) -> parser &
    {
        self.sv = s;
        return self; // NOLINT
    }
 
    std::string_view sv;
};
 
auto inline sep = parser<0>{default_sep};
auto inline end = parser<1>{default_end};
 
struct printer
{
    template<typename... T>
    auto print(T &&... args) const -> void
    {
        if constexpr(sizeof...(args) == 1) {
            using type = std::remove_cvref_t<std::tuple_element_t<0, std::tuple<T...> > >;
            if constexpr(std::is_same_v<type, parser<0> > or std::is_same_v<type, parser<1> >) {
                return;
            }
            if constexpr(requires {
                requires std::ranges::input_range<type>;
                requires not std::same_as<std::remove_cvref_t<type>, std::string>;
                requires not std::same_as<std::remove_cvref_t<type>, std::string_view>;
                requires not std::same_as<std::decay_t<type>, char *>;
            }) {
                std::ranges::for_each(std::forward<T>(args)..., [this]<typename U>(U &&arg) {
                    print(std::forward<U>(arg));
                });
            } else if constexpr(std::__tuple_like<type>) {
                auto f = [this]<typename... U>(U &&... arg) {
                    (print(std::forward<U>(arg)), ...);
                };
                std::apply(f, std::forward<T>(args)...);
            } else {
                ([this]<typename U>(U &&arg) {
                    if(flag) [[likely]] {
                        std::cout << sep;
                    }
                    flag = true;
                    std::cout << std::forward<U>(arg);
                }(std::forward<T>(args)), ...);
            }
        } else {
            (print(std::forward<T>(args)), ...);
        }
    }
 
    template<typename... T>
    auto operator()(T &&... args) const -> void
    {
        print(std::forward<T>(args)...);
        std::cout << end;
        sep.clear(), end.clear();
        flag = false;
    }
 
    bool mutable flag = false;
} constexpr print;