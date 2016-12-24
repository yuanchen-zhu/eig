// Plugin code to insert into definition of Eigen::Array.

#include <eig/plugins/eigen_plugin_common.hpp>

GRIT_ALIAS_EIGEN_DENSEBASE_TYPE_AND_CONSTS
GRIT_ALIAS_EIGEN_ARRAYBASE_TYPE_AND_CONSTS

public:
#define GRIT_EIGEN_THIS Array
#define GRIT_EIGEN_ROW Array<scalar, 1, static_cols, RowMajor>

#include <grit/adapt/internal/eigen_impl_list_construct.hpp>

#undef GRIT_EIGEN_THIS
#undef GRIT_EIGEN_ROW
