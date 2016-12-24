// Plugin code to insert into definition of Eigen's Matrix

#include <eig/plugins/eigen_plugin_common.hpp>

GRIT_ALIAS_EIGEN_DENSEBASE_TYPE_AND_CONSTS
GRIT_ALIAS_EIGEN_MATRIXBASE_TYPE_AND_CONSTS

public:
#define GRIT_EIGEN_THIS Matrix
#define GRIT_EIGEN_ROW Matrix<scalar, 1, static_cols, RowMajor>

#include <eig/plugins/eigen_impl_list_construct.hpp>

#undef GRIT_EIGEN_THIS
#undef GRIT_EIGEN_ROW


/// \}
