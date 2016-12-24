// Plugin code to insert into definition of Eigen::DenseBase.
// Define type, variable, and function alias acording to our code convention

#include <eig/plugins/eigen_plugin_common.hpp>

GRIT_ALIAS_EIGEN_DENSEBASE_TYPE_AND_CONSTS

GRIT_ALIAS_EIGEN_FN

// Acessing subblocks using range descriptors.

private:
template <typename T, typename = std::enable_if_t<grit::eig::is_range<T>>>
static inline grit::eig::index_span to_index_span(T r, Index size)
{
    return {r, size};
}

template <typename T,
          typename = std::enable_if_t<std::is_integral_v<T>>,
          typename = void>
static inline grit::eig::index_span to_index_span(T i, Index)
{
    return {static_cast<index>(i), 1};
}

template <class T>
static constexpr bool is_all_range =
    std::is_same_v<grit::eig::actual_range<T>, grit::eig::all_range>;

template <class R, index Extent>
static constexpr index block_static_size =
    std::is_integral_v<R>
        ? 1
        : is_all_range<R> ? Extent
                          : grit::eig::range_static_size_or<R, Dynamic>;

template <class RowRange, class ColRange>
static constexpr bool is_block_inner_panel = //
    (is_all_range<RowRange> && !is_row_major) ||
    (is_all_range<ColRange> && is_row_major);

template <class Derived, class RowRange, class ColRange>
using range_block = Block<Derived,
                          block_static_size<RowRange, static_rows>,
                          block_static_size<ColRange, static_cols>,
                          is_block_inner_panel<RowRange, ColRange>>;

template <class Derived, class Range>
using vector_range_block =
    VectorBlock<Derived, block_static_size<Range, static_size>>;

template <class T>
static constexpr bool is_range_or_int = grit::eig::is_range<T> || std::is_integral_v<T>;

public:
/// Create a submatrix view. `row_range` and `col_range` can be either a range
/// or an integral scalar, but not both scalars, or this method will be removed
/// from overload resolution to allow the regular `operator()` returning a
/// scalar to be used.
/// \{
template <class RowRange,
          class ColRange,
          class = std::enable_if_t<
              is_range_or_int<RowRange> && is_range_or_int<ColRange> &&
              !(std::is_integral_v<RowRange> && std::is_integral_v<ColRange>)>>
auto operator()(RowRange row_range, ColRange col_range) const
{
    auto r = to_index_span(row_range, rows());
    auto c = to_index_span(col_range, cols());
    return range_block<const Derived, RowRange, ColRange>(
        derived(), r.start, c.start, r.n, c.n);
}

template <class RowRange,
          class ColRange,
          class = std::enable_if_t<
              is_range_or_int<RowRange> && is_range_or_int<ColRange> &&
              !(std::is_integral_v<RowRange> && std::is_integral_v<ColRange>)>>
auto operator()(RowRange row_range, ColRange col_range)
{
    auto r = to_index_span(row_range, rows());
    auto c = to_index_span(col_range, cols());
    return range_block<Derived, RowRange, ColRange>(
        derived(), r.start, c.start, r.n, c.n);
}
/// \}

/// Create a subvector view
/// \{
template <class Range,
          class = std::enable_if_t<grit::eig::is_range<Range> && is_static_1d>>
auto operator()(Range range) const
{
    auto s = to_index_span(range, size());
    return vector_range_block<const Derived, Range>(derived(), s.start, s.n);
}

template <class Range,
          class = std::enable_if_t<grit::eig::is_range<Range> && is_static_1d>>
auto operator()(Range range)
{
    auto s = to_index_span(range, size());
    return vector_range_block<Derived, Range>(derived(), s.start, s.n);
}
/// \}
