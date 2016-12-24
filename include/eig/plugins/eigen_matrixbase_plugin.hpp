// Plugin code to insert into definition of Eigen::MatrixBase:
// Define type, variable, and function alias according to our code convention

#include <eig/plugins/eigen_plugin_common.hpp>

GRIT_ALIAS_EIGEN_DENSEBASE_TYPE_AND_CONSTS
GRIT_ALIAS_EIGEN_MATRIXBASE_TYPE_AND_CONSTS

GRIT_ALIAS_EIGEN_FN

template <int P>
decltype(auto) lp_norm() const
{
    return lpNorm<P>();
}
