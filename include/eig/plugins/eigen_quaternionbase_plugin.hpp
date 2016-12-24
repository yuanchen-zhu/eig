// Plugin code to insert into definition of Eigen's QuaternionBase: Define
// member type, variable, and function aliases to enforce our coding convention

#include <eig/plugins/eigen_plugin_common.hpp>

using scalar = Scalar;
using real_scalar = RealScalar;
using vec3 = Vector3;
using mat3 = Matrix3;
using angle_axis = AngleAxisType;

GRIT_ALIAS_EIGEN_MEM_CONST_FN(squared_norm, squaredNorm)
GRIT_ALIAS_EIGEN_MEM_CONST_FN(angular_distance, angularDistance)
GRIT_ALIAS_EIGEN_MEM_CONST_FN(to_rotation_mat, toRotationMatrix)
GRIT_ALIAS_EIGEN_MEM_CONST_FN(is_approx, isApprox)

GRIT_ALIAS_EIGEN_MEM_FN(set_identity, setIdentity)
GRIT_ALIAS_EIGEN_MEM_FN(set_from_two_vectors, setFromTwoVectors)

GRIT_ALIAS_EIGEN_STATIC_FN(identity, Identity)