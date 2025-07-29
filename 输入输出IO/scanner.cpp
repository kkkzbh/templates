struct scanner
{
    template<typename... T>
    auto operator()(T &&... args) const -> void
    {
        if constexpr(sizeof...(args) == 1) {
            using type = std::remove_cvref_t<std::tuple_element_t<0, std::tuple<T...> > >;
            if constexpr(requires {
                requires std::ranges::output_range<type, std::ranges::range_value_t<type> >;
                requires not std::same_as<std::remove_cvref_t<type>, std::string>;
            }) {
                std::ranges::for_each(std::forward<T>(args)..., *this);
            } else if constexpr(std::__tuple_like<type>) {
                std::apply(*this, std::forward<T>(args)...);
            } else {
                (std::cin >> ... >> args);
            }
        } else {
            ((*this)(args), ...);
        }
    }
} constexpr scan;