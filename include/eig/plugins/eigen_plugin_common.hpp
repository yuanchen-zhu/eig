#ifndef GRIT_EIGEN_PLUGIN_COMMON_HPP
#define GRIT_EIGEN_PLUGIN_COMMON_HPP

#define GRIT_ALIAS_EIGEN_DENSEBASE_TYPE_AND_CONSTS                             \
    using index = Index;                                                       \
    using scalar = Scalar;                                                     \
    using real_scalar = RealScalar;                                            \
    static constexpr index static_rows = RowsAtCompileTime;                    \
    static constexpr index static_cols = ColsAtCompileTime;                    \
    static constexpr index static_size = SizeAtCompileTime;                    \
    static constexpr index static_max_rows = MaxRowsAtCompileTime;             \
    static constexpr index static_max_cols = MaxColsAtCompileTime;             \
    static constexpr index static_max_size = MaxSizeAtCompileTime;             \
    static constexpr index static_inner_end = InnerSizeAtCompileTime;          \
    static constexpr index static_inner_stride = InnerStrideAtCompileTime;     \
    static constexpr index static_outer_stride = OuterStrideAtCompileTime;     \
    static constexpr bool is_static_1d = IsVectorAtCompileTime;                \
    static constexpr bool is_row_major = IsRowMajor;

#define GRIT_ALIAS_EIGEN_MATRIXBASE_TYPE_AND_CONSTS                            \
    using plain_object = PlainObject;

#define GRIT_ALIAS_EIGEN_ARRAYBASE_TYPE_AND_CONSTS                             \
    using plain_object = PlainObject;


#define GRIT_ALIAS_EIGEN_MEM_FN(alias_name, orig_name)                         \
    template <class... T>                                                      \
    EIGEN_STRONG_INLINE auto alias_name(T&&... args)                           \
        ->decltype(orig_name(std::forward<T>(args)...))                        \
    {                                                                          \
        return orig_name(std::forward<T>(args)...);                            \
    }

#define GRIT_ALIAS_EIGEN_MEM_CONST_FN(alias_name, orig_name)                   \
    template <class... T>                                                      \
    EIGEN_STRONG_INLINE auto alias_name(T&&... args)                           \
        const->decltype(orig_name(std::forward<T>(args)...))                   \
    {                                                                          \
        return orig_name(std::forward<T>(args)...);                            \
    }

#define GRIT_ALIAS_EIGEN_STATIC_FN(alias_name, orig_name)                      \
    template <class... T>                                                      \
    EIGEN_STRONG_INLINE static auto alias_name(T&&... args)                    \
        ->decltype(orig_name(std::forward<T>(args)...))                        \
    {                                                                          \
        return orig_name(std::forward<T>(args)...);                            \
    }


#define GRIT_ALIAS_EIGEN_FN                                                    \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(outer_size, outerSize)                       \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(inner_size, innerSize)                       \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(is_approx, isApprox)                         \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(is_much_smaller_than, isMuchSmallerThan)     \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(is_constant, isConstant)                     \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(is_zero, isZero)                             \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(is_ones, isOnes)                             \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(has_nan, hasNaN)                             \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(all_finite, allFinite)                       \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(min_coeff, minCoeff)                         \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(max_coeff, maxCoeff)                         \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(squared_norm, squaredNorm)                   \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(stable_norm, stableNorm)                     \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(blue_norm, blueNorm)                         \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(hypot_norm, hypotNorm)                       \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(as_diagonal, asDiagonal)                     \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(unary_expr, unaryExpr)                       \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(unary_view_expr, unaryViewExpr)              \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(binary_expr, binaryExpr)                     \
                                                                               \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(cwise_abs, cwiseAbs)                         \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(cwise_squared_abs, cwiseAbs2)                \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(cwise_sqrt, cwiseSqrt)                       \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(cwise_inverse, cwiseInverse)                 \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(cwise_eq, cwiseEqual)                        \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(cwise_neq, cwiseNotEqual)                    \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(cwise_min, cwiseMin)                         \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(cwise_max, cwiseMax)                         \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(cwise_product, cwiseProduct)                 \
    GRIT_ALIAS_EIGEN_MEM_CONST_FN(cwise_quotient, cwiseQuotient)               \
                                                                               \
    GRIT_ALIAS_EIGEN_MEM_FN(adjoint_in_place, adjointInPlace)                  \
    GRIT_ALIAS_EIGEN_MEM_FN(transpose_in_place, transposeInPlace)              \
    GRIT_ALIAS_EIGEN_MEM_FN(set_constant, setConstant)                         \
    GRIT_ALIAS_EIGEN_MEM_FN(set_linspace, setLinSpaced)                        \
    GRIT_ALIAS_EIGEN_MEM_FN(set_zero, setZero)                                 \
    GRIT_ALIAS_EIGEN_MEM_FN(set_ones, setOnes)                                 \
    GRIT_ALIAS_EIGEN_MEM_FN(set_random, setRandom)                             \
    GRIT_ALIAS_EIGEN_MEM_FN(reverse_in_place, reverseInPlace)                  \
                                                                               \
    GRIT_ALIAS_EIGEN_STATIC_FN(identity, Identity)                             \
    GRIT_ALIAS_EIGEN_STATIC_FN(unit, Unit)                                     \
    GRIT_ALIAS_EIGEN_STATIC_FN(unit_x, UnitX)                                  \
    GRIT_ALIAS_EIGEN_STATIC_FN(unit_y, UnitY)                                  \
    GRIT_ALIAS_EIGEN_STATIC_FN(unit_z, UnitZ)                                  \
    GRIT_ALIAS_EIGEN_STATIC_FN(unit_w, UnitW)                                  \
    GRIT_ALIAS_EIGEN_STATIC_FN(constant, Constant)                             \
    GRIT_ALIAS_EIGEN_STATIC_FN(linspace, LinSpaced)                            \
    GRIT_ALIAS_EIGEN_STATIC_FN(nullary_expr, NullaryExpr)                      \
    GRIT_ALIAS_EIGEN_STATIC_FN(zero, Zero)                                     \
    GRIT_ALIAS_EIGEN_STATIC_FN(ones, Ones)                                     \
    GRIT_ALIAS_EIGEN_STATIC_FN(random, Random)


#endif