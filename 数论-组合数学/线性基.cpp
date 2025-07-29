// C++17

namespace linear_basis
{

    namespace xorsp
    {
        /* 异或空间
         * 1. 任意a,b 用a^b代替a,b中任意一个，不影响空间的构成
         * 2. 任意a,b 如果有a^b = 0，舍弃任意一个，不影响空间构成
         * 3. 异或空间是否存0，需要单独标记
         */

        template<typename T>
        struct is_bitset : std::false_type {};

        template<std::size_t N>
        struct is_bitset<std::bitset<N>> : std::true_type {};

        template<typename T>
        auto constexpr test(T const& v,int i) -> bool
        {
            /*
             * 测试v的第i位 (0-based)
             * 值为 0 或者 1
             */
            return v >> i & 1;
        }

        template<typename T>
        auto constexpr set(T& v,int i) -> void
        {
            v |= T{ 1 } << i;
        }

        template<typename T>
        auto constexpr reset(T& v,int i) -> void
        {
            v &= ~(T{ 1 } << i);
        }

        template<typename T>
        auto constexpr insert(std::vector<T> &a, int m, T v) -> std::pair<bool,int>
        {
            /*
             * 异或空间
             * 插入异或
             * a:       整数序列
             * m:       整数位数
             * v:       插入元素 (大于0)
             * return:
             *   bool: 插入是否成功(表示有无0的发生)
             *   int: 插入的位置
             * requires: a中的数从小到大排放，值为0表示空, a[i]若不空, a[i]第i位(0-based)必为1
             */
            for(auto i = m; i--; ) {
                if(not test(v,i)) {
                    continue;
                }
                if(not a[i]) {
                    a[i] = v;
                    return std::make_pair(true,i);
                }
                v ^= a[i];
            }
            return std::make_pair(false,-1);
        }

        template<typename T,typename W,typename Cmp = std::greater<>,typename Proj = std::identity>
        auto constexpr insert(std::vector<T>& a,std::vector<W>& t,int m,T v,W weight,Cmp cmp = std::greater{},Proj proj = std::identity{})
        {
            /*
             * 带权插入异或
             * t: 该线性基的权值序列
             * weight: 权值
             * cmp, proj: 比较器
             * requires: 与非带权的 insert 一致
             * 从行为上来讲，如果 cmp(new,old)为true 那么会发生替换，即基内会尽可能留下排序序前的权值
             */
            for(auto i = m; i--; ) {
                if(not test(v,i)) {
                    continue;
                }
                if(not a[i]) {
                    a[i] = v;
                    t[i] = weight;
                    return;
                }
                if(cmp(proj(weight),proj(t[i]))) {
                    std::swap(weight,t[i]);
                    std::swap(v,a[i]);
                }
                v ^= a[i];
            }
        }

        template<typename T>
        auto constexpr contains(std::vector<T> const& a,int m,bool zero,T v) -> bool
        {
            /*
             * 异或空间
             * 检查该空间是否存在异或 v
             */
            if(zero and not v) {
                return true;
            }
            for(auto i = m; i != -1; --i) {
                if(test(v,i) and not (v ^= a[i])) {
                    return true;
                }
            }
            return false;
        }

        template<typename T>
        auto constexpr increment(std::vector<T> const &a, int m) -> std::tuple<std::vector<T>, bool,int>
        {
            /*
             * 异或空间
             * 自增法求线性基 (副对角线)
             * return:
             *   std::vector<T>: 线性基
             *   bool: 是否含 0
             *   int: 基的个数
             */
            auto ret = std::vector(m + 1,T{});
            auto ct = 0;
            for(auto v : a) {
                ct += insert(ret, m, v);
            }
            return std::make_tuple(ret, ct != a.size(),ct);
        }

        template<typename T>
        auto constexpr shrink(std::vector<T>& a) -> void
        {
            /*
             * shrink 基数组
             * requires: 基数组满足 insert 的requires要求
             */
            auto it = std::remove(a.begin(),a.end(),0);
            a.erase(it,a.end());
        }

        template<typename T>
        auto constexpr normalization(std::vector<T>& a)
        {
            /*
             * 异或空间
             * 基于基a返回一个标准化处理后的基，该基异或空间与原基相同
             * requires: a 是 shrink 的
             */
            for(auto i = a.size(); i--; ) {
                for(auto j = i - 1; j != a.size(); ++j) {
                    a[j] = std::min(a[j],a[j] ^ a[i]);
                }
            }
        }

        template<typename T>
        auto constexpr max(std::vector<T> const& a) -> T
        {
            /*
             * 异或空间
             * 异或最大值
             * requires: 基a 为副对角线下三角
             */
            auto ret = T{};
            for(auto i = a.size(); i--; ) {
                ret = std::max(ret,ret ^ a[i]);
            }
            return ret;
        }

        template<typename T>
        auto constexpr min(std::vector<T> const& a,bool zero) -> T
        {
            /*
             * 异或空间
             * 最小值 (基于贪心)
             * requires: 与 insert 一致
             */
            if(zero) {
                return 0;
            }
            for(auto i = 0; i != a.size(); ++i) {
                if(not a[i]) {
                    continue;
                }
                return a[i];
            }
            return 0;
        }

        template<typename T>
        auto constexpr size(std::vector<T> const& a,bool zero) -> u64
        {
            /*
             * return: 一个异或空间能表示的数的个数
             */
            return (1ULL << a.size()) - not zero;
        }

        template<typename T>
        auto constexpr kmin(std::vector<T> const& a,bool zero,u64 k) -> T
        {
            /*
             * 异或和第 k 小值 (1-based)
             * a: 标准副对角线基
             * return: 若 k 超出能表示的范围 返回-1
             */
            if(k > size(a,zero)) {
                return -1;
            }
            k -= zero;
            auto c = T{};
            while(k) {
                auto i = std::__countr_zero(k);
                c ^= a[i];
                reset(k,i);
            }
            return c;
        }

        template<typename T>
        auto constexpr gauss(std::vector<T>& a,int m) -> bool
        {
            /*
             * 异或空间
             * 高斯削元求**标准**线性基 (副对角线)
             * a: 整数序列 (最终被修改为标准副对角线基)
             * m: 位数
             * return: 空间是否含0
             */
            auto n = int(a.size());
            auto rank = 0;
            for(auto i = m; i-- and rank != n;) { // 迭代，每次迭代 1 位 是一行也是一列
                auto it = rank;
                for(; it != n; ++it) {
                    if(test(a[it],i)) {
                        break;
                    }
                }
                if(it == n) {
                    continue;
                }
                std::swap(a[rank],a[it]);
                for(auto k = 0; k != n; ++k) {
                    if(k == rank or not test(a[k],i)) {
                        continue;
                    }
                    a[k] ^= a[rank];
                }
                ++rank;
            }
            a.resize(rank);
            std::reverse(a.begin(),a.end());
            return rank != n;
        }
    }

    namespace vecsp
    {
        /*
         * 向量空间
         */

        auto constexpr eps = 1e-8;

        auto test(double v) -> bool
        {
            return std::abs(v) >= eps;
        }

        template<typename T>
        auto constexpr gauss(std::vector<std::vector<T>>& a) -> std::vector<int>
        {
            /*
             * **普通**高斯削元 (副对角线)
             * return:
             *   mask: 从 a 中选用作为基的向量的索引值
             */
            auto n = a.size();
            auto m = a[0].size();
            auto rank = 0;
            auto mask = std::vector<int>{};
            for(auto i = 0; i != m and rank != n; ++i) {
                auto it = rank;
                for(; it != n; ++it) {
                    if(test(a[it][i])) {
                        break;
                    }
                }
                if(it == n) {
                    continue;
                }
                mask.emplace_back(it);
                std::swap(a[rank],a[it]);
                for(auto k = 0; k != n; ++k) {
                    if(k == rank or not test(a[k][i])) {
                        continue;
                    }
                    auto ratio = a[k][i] / a[rank][i];
                    for(auto cx = 0; cx != m; ++cx) {
                        a[k][cx] -= ratio * a[rank][cx];
                    }
                }
                ++rank;
            }
            return mask;
        }

    }
}