/// \file 
/// Implement list constructors used for both array and matrix. There is no
/// inclusion guard because this file can be included multiple times for
/// different types
///
/// The following macros are assumed to be defined
/// - GRIT_EIGEN_THIS: the name of the type including this file
/// - GRIT_EIGEN_ROW: the name of the type corresponding to a 1D row

/// Construct with an initial size for 2d matrix or array.
/// \pre `*this` is a dynamically sized, and not static 1d

#ifndef GRIT_EIGEN_THIS
#error GRIT_EIGEN_THIS needs to be defined before including this file
#endif

#ifndef GRIT_EIGEN_ROW
#error GRIT_EIGEN_ROW needs to be defined before including this file
#endif

EIGEN_STRONG_INLINE GRIT_EIGEN_THIS(::eig::from_size_tag,
                                    index rows,
                                    index cols)
{
    static_assert(!is_static_1d,
                  "Cannot specify both rows and cols for a "
                  "compile-time 1d matrix or array");
    static_assert(static_size == Dynamic,
                  "cannot size construct a statically sized matrix or array");
    resize(rows, cols);
}

/// Construct with an initial size for column or row vectors. Allow length to be
/// any type because Eigen's constructor takes in two arbitrary typed parameter
/// to resolve ambiguity between ctor(x, y) and ctor(rows, cols), and we
/// need to be prioritized over it when the first argument is the sized
/// construction tag.
///
/// \pre `*this` is a dynamically sized, and static 1d
template <typename Length>
EIGEN_STRONG_INLINE GRIT_EIGEN_THIS(::eig::from_size_tag, Length length)
{
    static_assert(std::is_integral_v<Length>,
                  "length parameter must be an integer type");

    static_assert(static_size == Dynamic,
                  "unitialized_alloc construct only works for dynamically "
                  "sized matrix or array");

    static_assert(is_static_1d,
                  "Only compile time 1d matrix or vector can be "
                  "constructed from length alone.");

    resize(length);
}

/// \name Construct from 1d list
/// Construct a statically 1D matrix or array from 1d list of the form
/// ```
///     vec{a, b, c, ...};
///     array{a, b, c, ...};
/// ```
/// If the size of the vector is specified at compile time, then the number of
/// argument passed in must agree with the compile-time size, or it is a
/// compile-time error.
///
/// Because the arguments are passed in either wrapped in an initializer_list
/// (when dynamically sized) or as multiple arguments (when statically sized),
/// when using parenthesis based calling syntax, there will be a slight
/// difference, i.e.,
/// ```
///    vec(a, b, c);  // statically sized vec({a, b, c}); // dynamically sized
/// ```
/// \{
template <typename = std::enable_if_t<static_size == Dynamic>>
EIGEN_STRONG_INLINE GRIT_EIGEN_THIS(std::initializer_list<scalar> values)
{
    // allocate space (or do nothing for static sized object)
    resize(values.size());

    index i = 0;
    for (auto&& x : values)
        (*this)[i++] = static_cast<scalar>(x);
}

private:
EIGEN_STRONG_INLINE void assign_1d_list(index i) {}

template <typename... T>
EIGEN_STRONG_INLINE void assign_1d_list(index i, scalar first, T... rest)
{
    (*this)[i] = first;
    assign_1d_list(i + 1, rest...);
}

public:
// Enable when static_size == 1 or > 4, since base Eigen already defines
// constructing with 1, 2, 3, and 4 elements. However constructing with 1
// element is an explicit constructor. We want to allow implicitly construct 1x1
// matrix/array from a scalar.
template <typename... Rest,
          typename = std::enable_if_t<
              static_size != Dynamic && sizeof...(Rest) == static_size - 1 &&
              (static_size == 1 || static_size > 4) && is_static_1d>>
EIGEN_STRONG_INLINE GRIT_EIGEN_THIS(scalar first_value, Rest... rest_values)
{
    assign_1d_list(0, first_value, rest_values...);
}
/// \}

/// \name Construct from 2d list
/// Construct a matrix or array from 2d list literal of the form (where T can be
/// matrix or array)
/// ```
///     T{row_1,
///       row_2
///       ...,
///       row_n};
/// ```
///
/// Each row is something convertable to a 1d vector, and can be another braced
/// list, i.e.,
/// ```
///     T{{a, b},
///       {c, d}};
/// ```
///
/// A mismatch between the input #cols and static_cols (if specified) will
/// result in a compile time error.
///
/// A mismatch between the input #rows and static_rows (if specified) is
/// detected at runtime and results in assertion error.
///
/// \todo My efforts in making the check compile-time have failed so far.
///
/// Note that temporaries can get created when creating a temporary
/// initializer_list to store the rows for dynamically sized matrices, whereas
/// an optimizing compiler should be able to avoid creating temporaries for
/// statically sized matrices.

template <typename = std::enable_if_t<static_rows != 1>>
EIGEN_STRONG_INLINE GRIT_EIGEN_THIS(std::initializer_list<GRIT_EIGEN_ROW> values)
{
    index rows = values.size();
    index cols = rows > 0 ? values.begin()[0].size() : 0;
    resize(rows, cols); // does nothing for static sized matrix

    index r = 0;
    for (auto&& src_row : values)
        row(r++) = src_row;
}

#if 0 // failed attemp at checking input #row at compile-time

private:

inline void mat_assign_rows(index i) {}

template <typename First, typename... Rest>
inline void mat_assign_rows(index i, First&& first, Rest&&... rest)
{
    row(i) = Matrix<scalar, 1, static_cols, RowMajor>{std::forward<First>(first)};
    mat_assign_rows(i+1, std::forward<Rest>(rest)...);
}

template<typename T, index N>
struct fixed_list {
    template <typename... U,
              typename = std::enable_if_t<N != Dynamic && sizeof...(U) == N>>
    fixed_list(U... u)
    {
    }

    template <typename = std::enable_if_t<N == Dynamic>>
    fixed_list(std::initializer_list<T>)
    {
    }

private:
    fixed_list(int);
};

public:

template <typename... T,
          typename = std::enable_if_t<static_rows != Dynamic &&
                                      (sizeof...(T) == static_rows)>>
EIGEN_STRONG_INLINE Matrix(
    const fixed_list<T, static_cols>&... rows)
{
    //mat_assign_rows(0, std::forward<Row>(first_row), std::forward<RestRows>(rest_rows)...);
}
#endif