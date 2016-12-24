// Plugin code to insert into definition of Eigen's MatrixBase:
// Define type, variable, and function alias according to our naming convention

#include <eig/plugins/eigen_plugin_common.hpp>

using scalar = Scalar;
using vec = VectorType;
using mat = MatrixType;

GRIT_ALIAS_EIGEN_MEM_CONST_FN(squared_norm, squaredNorm)
GRIT_ALIAS_EIGEN_MEM_CONST_FN(is_approx, isApprox)

GRIT_ALIAS_EIGEN_MEM_FN(set_identity, setIdentity)
GRIT_ALIAS_EIGEN_MEM_FN(make_affine, makeAffine)

GRIT_ALIAS_EIGEN_STATIC_FN(identity, Identity)
