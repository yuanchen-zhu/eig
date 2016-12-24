#ifndef GRIT_EIGEN_HPP
#define GRIT_EIGEN_HPP

#include <limits>
#include <stdexcept>
#include <type_traits>

#if defined(EIGEN_CORE_H) || defined(EIGEN_MATRIXBASE_H) ||                    \
    defined(EIGEN_DENSEBASE_H) || defined(EIGEN_MATRIX_H) ||                   \
    defined(EIGEN_QUATERNIONBASE_H) || defined(EIGEN_TRANSFORM_H) ||           \
    defined(EIGEN_ARRAYBASE_H) || defined(EIGEN_ARRAY_H)
#error "Eigen headers must not be included before this header."
#endif

// clang-format off
#define EIGEN_DENSEBASE_PLUGIN      <eig/plugins/eigen_densebase_plugin.hpp>
#define EIGEN_ARRAYBASE_PLUGIN      <eig/plugins/eigen_arraybase_plugin.hpp>
#define EIGEN_ARRAY_PLUGIN          <eig/plugins/eigen_array_plugin.hpp>
#define EIGEN_MATRIXBASE_PLUGIN     <eig/plugins/eigen_matrixbase_plugin.hpp>
#define EIGEN_MATRIX_PLUGIN         <eig/plugins/eigen_matrix_plugin.hpp>
#define EIGEN_QUATERNIONBASE_PLUGIN <eig/plugins/eigen_quaternionbase_plugin.hpp>
#define EIGEN_TRANSFORM_PLUGIN      <eig/plugins/eigen_transform_plugin.hpp>
// clang-format on

/// Encapsulate Eigen specific types and functions
namespace eig {

    /// \name Construct from size
    /// Construct dynamically-sized Eigen matrices using a tag and actual size.
    /// The tag is useful since without it, the syntax can be confused with
    /// construction from actual values. Example usage:
    /// \snippet mat_test.cpp construct_from_size
    /// \{
    struct from_size_tag {
    };
    constexpr from_size_tag from_size = {};
    /// \}

    /// \name Ranged indexing
    /// Use index ranges to provide a cleaner and unified interface for
    /// describing submatrices, and can replace Eigen's army of subblock access
    /// functions:
    /// - block(),
    /// - {upper|lower}{Left|Right}Corner()
    /// - top|bottom|middle}Rows()
    /// - {left|middle|right}Cols()
    /// - head()
    /// - tail()
    /// - segment().
    ///
    /// Example usage:
    /// \snippet mat_test.cpp block_access_example
    ///
    /// Note that there are two versions of `first`, `last`, and `range`. One
    /// that takes in static template parameters indicating the size of the
    /// range at compile time, and one that allows the size to be determined at
    /// runtime.
    /// \{
    namespace detail {
        struct base_range {
        };

        struct has_static_size_tag {
        };

        template <std::ptrdiff_t N>
        struct base_static_sized_range : base_range, has_static_size_tag {
            static constexpr std::ptrdiff_t static_size = N;
        };
    }

    struct all_range : detail::base_range {
    };

    struct head_range : detail::base_range {
        std::ptrdiff_t n;
        head_range(std::ptrdiff_t n) : n{n} {}
    };

    struct tail_range : detail::base_range {
        std::ptrdiff_t n;
        tail_range(std::ptrdiff_t n) : n{n} {}
    };

    struct contiguous_range : detail::base_range {
        std::ptrdiff_t start, n;
        contiguous_range(std::ptrdiff_t start, std::ptrdiff_t n) : start{start}, n{n} {}
    };

    template <std::ptrdiff_t N>
    struct static_sized_contiguous_range : detail::base_static_sized_range<N> {
        std::ptrdiff_t start;
        static_sized_contiguous_range(std::ptrdiff_t start) : start{start} {}
    };

    template <std::ptrdiff_t N>
    struct static_head_range : detail::base_static_sized_range<N> {
    };

    template <std::ptrdiff_t N>
    struct static_tail_range : detail::base_static_sized_range<N> {
    };

    template <std::ptrdiff_t Start, std::ptrdiff_t Finish>
    struct static_range : detail::base_static_sized_range<Finish - Start + 1> {
    };

    constexpr all_range all{};
    inline head_range first(std::ptrdiff_t n) { return {n}; }
    inline tail_range last(std::ptrdiff_t n) { return {n}; }
    inline contiguous_range range(int start, int finish)
    {
        return {start, finish - start + 1};
    }
    template <std::ptrdiff_t N>
    inline static_sized_contiguous_range<N> static_sized_range(int start)
    {
        return {start};
    }

    template <std::ptrdiff_t N>
    inline static_head_range<N> first()
    {
        return {};
    };

    template <std::ptrdiff_t N>
    inline static_tail_range<N> last()
    {
        return {};
    };

    template <std::ptrdiff_t Start, std::ptrdiff_t Finish>
    inline static_range<Start, Finish> range()
    {
        return {};
    };

    namespace detail {
        template <class T, bool InvokeResultOf>
        struct actual_range_impl {
            using type = T;
        };

        template <class T>
        struct actual_range_impl<T, true> {
            using type = std::result_of_t<T()>;
        };

        template <class T, std::ptrdiff_t Default, bool HasStaticSize>
        struct range_static_size_or_impl {
            static constexpr std::ptrdiff_t value = Default;
        };

        template <class T, std::ptrdiff_t Default>
        struct range_static_size_or_impl<T, Default, true> {
            static constexpr std::ptrdiff_t value = T::static_size;
        };
    }

    /// Return real type of value returned by a range literal, possibly passing
    /// through a pointer to function and a funciont result.
    template <class T>
    using actual_range = typename detail::actual_range_impl<
        T,
        std::is_pointer_v<T> &&
            std::is_function_v<std::remove_pointer_t<T>>>::type;

    template <class T>
    constexpr bool is_range =
        std::is_base_of_v<detail::base_range, actual_range<T>>;

    template <class T>
    constexpr bool has_static_size =
        std::is_base_of_v<detail::has_static_size_tag, actual_range<T>>;

    template <class T, std::ptrdiff_t Default>
    constexpr std::ptrdiff_t range_static_size_or =
        detail::range_static_size_or_impl<actual_range<T>,
                                          Default,
                                          has_static_size<T>>::value;

    struct index_span {
        std::ptrdiff_t start, n;

        index_span(std::ptrdiff_t start, std::ptrdiff_t n) : start{start}, n{n} {}

        index_span(all_range r, std::ptrdiff_t size) : index_span{0, size} {}

        index_span(head_range r, std::ptrdiff_t) : index_span{0, r.n} {}

        index_span(tail_range r, std::ptrdiff_t size) : index_span{size - r.n, r.n} {}

        index_span(contiguous_range r, std::ptrdiff_t) : index_span{r.start, r.n} {}

        template <std::ptrdiff_t N>
        index_span(static_sized_contiguous_range<N> r, std::ptrdiff_t)
        : index_span{r.start, N}
        {
        }

        template <std::ptrdiff_t N>
        index_span(static_head_range<N>, std::ptrdiff_t) : index_span{0, N}
        {
        }

        template <std::ptrdiff_t N>
        index_span(static_tail_range<N>, std::ptrdiff_t size) : index_span{size - N, N}
        {
        }

        template <std::ptrdiff_t Start, std::ptrdiff_t Finish>
        index_span(static_range<Start, Finish>, std::ptrdiff_t)
        : index_span{Start, Finish - Start + 1}
        {
        }

        template <std::ptrdiff_t N>
        index_span(static_head_range<N> (*)(), std::ptrdiff_t) : index_span{0, N}
        {
        }

        template <std::ptrdiff_t N>
        index_span(static_tail_range<N> (*)(), std::ptrdiff_t size)
        : index_span{size - N, N}
        {
        }

        template <std::ptrdiff_t Start, std::ptrdiff_t Finish>
        index_span(static_range<Start, Finish> (*)(), std::ptrdiff_t)
        : index_span{Start, Finish - Start + 1}
        {
        }
    };
    /// \}
}
}

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace eig {
    static_assert(std::is_same_v<std::ptrdiff_t, Eigen::DenseIndex>,
                  "std::ptrdiff_t must be the same as Eigen's index type");

    /// \name Alias of Eigen types and values, with improved interfaces
    /// \{
    constexpr std::ptrdiff_t dynamic = Eigen::Dynamic;

    template <typename D>
    using dense_base = Eigen::DenseBase<D>;

    template <typename D>
    using array_base = Eigen::ArrayBase<D>;

    template <class D>
    using mat_base = Eigen::MatrixBase<D>;

    enum class storage { column_major, row_major };
    enum class alignment { auto_align, dont_align };
    enum class transform_type { affine_full, affine_compact, projective };

    namespace detail {
        inline constexpr auto to_eigen_option(storage s)
        {
            return s == storage::column_major ? Eigen::ColMajor
                                              : Eigen::RowMajor;
        }

        inline constexpr auto to_eigen_option(alignment a)
        {
            return a == alignment::auto_align ? Eigen::AutoAlign
                                              : Eigen::DontAlign;
        }

        inline constexpr auto to_eigen_transform_type(transform_type t)
        {
            return t == transform_type::affine_full
                       ? Eigen::Affine
                       : (t == transform_type::affine_compact
                              ? Eigen::AffineCompact
                              : Eigen::Projective);
        }
    }

    /// 2D Numerical array, imported from Eigen. Note that statically sized
    /// float/double arrays whose size in bytes are multiples of 16 bytes must
    /// be 16 bytes aligned, since Eigen use SIMD instructros to manipulate
    /// them. So will need custom aligned allocaters when put into STL
    /// containers and dynamically allocating them.
    ///
    ///
    /// \todo maybe instead of directly import, create own class that
    ///       wraps around Eigen's, so have better control on construction
    ///       syntax and function call naming
    template <typename Scalar,
              std::ptrdiff_t Rows = dynamic,
              std::ptrdiff_t Cols = dynamic,
              storage Storage = storage::column_major,
              alignment Alignment = alignment::auto_align>
    using array2 = Eigen::Array<Scalar,
                                Rows,
                                Cols,
                                detail::to_eigen_option(Storage) |
                                    detail::to_eigen_option(Alignment)>;

    /// 1D numerical array.
    template <typename Scalar,
              std::ptrdiff_t Size = dynamic,
              alignment Alignment = alignment::auto_align>
    using array = array2<Scalar, Size, 1, storage::column_major, Alignment>;

    /// Matrix, imported from Eigen. Note that statically sized float/double
    /// matrices whose size in bytes are multiples of 16 bytes must be 16 bytes
    /// aligned, since Eigen use SIMD instructros to manipulate them. So will
    /// need custom aligned allocaters when put into STL containers and
    /// dynamically allocating them.
    ///
    /// \todo maybe instead of directly import, create own class that
    ///       wraps around Eigen's, so have better control on construction
    ///       syntax and function call naming
    template <typename Scalar,
              std::ptrdiff_t Rows = dynamic,
              std::ptrdiff_t Cols = dynamic,
              storage Storage = storage::column_major,
              alignment Alignment = alignment::auto_align>
    using mat = Eigen::Matrix<Scalar,
                              Rows,
                              Cols,
                              detail::to_eigen_option(Storage) |
                                  detail::to_eigen_option(Alignment)>;

    /// A column vector. Eigen's convention is column vecs are column majored.
    template <typename Scalar,
              std::ptrdiff_t Size = eig::dynamic,
              alignment Alignment = alignment::auto_align>
    using vec = mat<Scalar, Size, 1, storage::column_major, Alignment>;

    /// A row vector. Eigen's convention is row vecs are row majored
    template <typename Scalar,
              std::ptrdiff_t Size = eig::dynamic,
              alignment Alignment = alignment::auto_align>
    using row_vec = mat<Scalar, 1, Size, storage::row_major, Alignment>;

    /// A perspective or affine transform imported from Eigen
    template <typename Scalar,
              std::ptrdiff_t Dim,
              transform_type Type,
              storage Storage = storage::column_major,
              alignment Alignment = alignment::auto_align>
    using transform = Eigen::Transform<Scalar,
                                       Dim,
                                       detail::to_eigen_transform_type(Type),
                                       detail::to_eigen_option(Storage) |
                                           detail::to_eigen_option(Alignment)>;

    /// A quaternion representing 3d rotation imported from Eigen
    template <typename Scalar, alignment Alignment = alignment::auto_align>
    using quat = Eigen::Quaternion<Scalar, detail::to_eigen_option(Alignment)>;

    /// \}
}

namespace Eigen {

/// \name Allow syntax like `dot(a, b)` when `a.dot(b)` is defined
//  \{
/// Allow syntax like `FN(a, b)` if `a.FN(b)` is defined, by injecting free
/// standing versions of Eigen class member functions in the Eigen namespace.
/// These functions can automatically be looked up via arugment dependent
/// lookup. `BASE` is one of the Eigen base types such as `MatrixBase`.
#define GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, FN)                           \
    template <class D, class... T>                                             \
    inline auto FN(const BASE<D>& obj, T&&... args)                            \
        ->decltype(obj.FN(std::forward<T>(args)...))                           \
    {                                                                          \
        return obj.FN(std::forward<T>(args)...);                               \
    }

/// Export a list of functions that allow non-member invokation for a
/// specific Eigen base type.
#define GRIT_EXPORT_FREE_STANDING_EIGN_FN_FOR_BASE(BASE)                       \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, is_approx)                        \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, is_constant)                      \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, is_ones)                          \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, is_zero)                          \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, prod)                             \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, reverse)                          \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, sum)                              \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, transpose)                        \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cwise_abs)                        \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cwise_squared_abs)                \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cwise_sqrt)                       \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cwise_inverse)                    \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cwise_eq)                         \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cwise_neq)                        \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cwise_min)                        \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cwise_max)                        \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cwise_product)                    \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cwise_quotient)                   \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, normalized)                       \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, norm)                             \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, inverse)                          \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, conjugate)                        \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, adjoint)                          \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, dot)                              \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cross)                            \
    GRIT_EXPORT_FREE_STANDING_EIGEN_FN(BASE, cross3)


GRIT_EXPORT_FREE_STANDING_EIGN_FN_FOR_BASE(DenseBase)
GRIT_EXPORT_FREE_STANDING_EIGN_FN_FOR_BASE(ArrayBase)
GRIT_EXPORT_FREE_STANDING_EIGN_FN_FOR_BASE(MatrixBase)

#undef GRIT_EXPORT_FREE_STANDING_EIGEN_FN_FOR_BASE
#undef GRIT_EXPORT_FREE_STANDING_EIGEN_FN
/// \}

/// \name Allow `scalar +/- matrix'
/// \{
/// Allow `matrix OP scalar` and `scalar OP matrix` by upgrading the scalar to a
/// constant matrix and then invoking the OP.
#define GRIT_EXPORT_MATRIX_SCALAR_OP(OP)                                       \
    template <typename D, typename Scalar>                                     \
    auto operator OP(const MatrixBase<D>& m, Scalar&& s)                       \
        ->decltype(                                                            \
            m OP D::constant(m.rows(), m.cols(), std::forward<Scalar>(s)))     \
    {                                                                          \
        return m OP D::constant(m.rows(), m.cols(), std::forward<Scalar>(s));  \
    }                                                                          \
    template <typename D, typename Scalar>                                     \
    auto operator OP(Scalar&& s, const MatrixBase<D>& m)                       \
        ->decltype(D::constant(m.rows(), m.cols(), std::forward<Scalar>(s))    \
                       OP m)                                                   \
    {                                                                          \
        return D::constant(m.rows(), m.cols(), std::forward<Scalar>(s)) OP m;  \
    }

GRIT_EXPORT_MATRIX_SCALAR_OP(+)
GRIT_EXPORT_MATRIX_SCALAR_OP(-)
#undef GRIT_EXPORT_MATRIX_SCALAR_OP
/// \}
}


#endif