/*
 * Copyright (c) 2013, Christian Gehring, Hannes Sommer, Paul Furgale, Remo Diethelm
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autonomous Systems Lab, ETH Zurich nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Christian Gehring, Hannes Sommer, Paul Furgale,
 * Remo Diethelm BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include "kindr/common/gtest_eigen.hpp"
#include "kindr/quaternions/QuaternionEigen.hpp"
#include "kindr/rotations/RotationEigen.hpp"
#include <string>

namespace rot = kindr::rotations::eigen_impl;
namespace quat = kindr::quaternions::eigen_impl;



template <typename ImplementationPair_>
struct ConversionTest : public ::testing::Test{
  typedef typename ImplementationPair_::first_type RotationA;
  typedef typename ImplementationPair_::second_type RotationB;

  RotationA rotA;
  RotationB rotB;

  double tol = 1e-4;
};

template <typename ImplementationPair_>
struct ConcatenationTest : public ConversionTest<ImplementationPair_> {

};

template <typename Rotation_>
struct RotationQuaternionTestType {
  typedef Rotation_ Rotation;
  typedef typename Rotation::Scalar Scalar;

  const Rotation rotQuarterX = Rotation(1/sqrt(2.0),1/sqrt(2.0),0.0,0.0);
  const Rotation rotQuarterY = Rotation(1/sqrt(2.0),0.0,1/sqrt(2.0),0.0);
  const Rotation rotQuarterZ = Rotation(1/sqrt(2.0),0.0,0.0,1/sqrt(2.0));
  const Rotation rotIdentity = Rotation(1.0,0.0,0.0,0.0);
  const Rotation rotGeneric = Rotation(quat::Quaternion<Scalar>(2.0, 3.0, 4.0, 5.0).toUnitQuaternion());

  Rotation rot;

  void assertNear(const Rotation_& rotA, const Rotation_& rotB, double tol=1e-6, const std::string& msg = "") {
    ASSERT_NEAR(rotA.w(), rotB.w(), tol) << msg;
    ASSERT_NEAR(rotA.x(), rotB.x(), tol) << msg;
    ASSERT_NEAR(rotA.y(), rotB.y(), tol) << msg;
    ASSERT_NEAR(rotA.z(), rotB.z(), tol) << msg;
  }
};


template <typename Rotation_>
struct RotationVectorTestType {
  typedef Rotation_ Rotation;
  typedef typename Rotation::Scalar Scalar;

  const Rotation rotQuarterX = Rotation(M_PI/2.0,0.0,0.0);
  const Rotation rotQuarterY = Rotation(0.0,M_PI/2.0,0.0);
  const Rotation rotQuarterZ = Rotation(0.0,0.0,M_PI/2.0);
  const Rotation rotIdentity = Rotation(0.0,0.0,0.0);
  const Rotation rotGeneric = Rotation(2.0,3.0,4.0);

  Rotation rot;

  void assertNear(const Rotation_& rotA, const Rotation_& rotB, double tol=1e-6, const std::string& msg = "") {
    ASSERT_NEAR(rotA.x(), rotB.x(), tol) << msg;
    ASSERT_NEAR(rotA.y(), rotB.y(), tol) << msg;
    ASSERT_NEAR(rotA.z(), rotB.z(), tol) << msg;
  }
};

template <typename Rotation_>
struct AngleAxisTestType {
  typedef Rotation_ Rotation;
  typedef typename Rotation::Scalar Scalar;

  const Rotation rotQuarterX = Rotation(M_PI/2.0, 1.0, 0.0, 0.0);
  const Rotation rotQuarterY = Rotation(M_PI/2.0, 0.0, 1.0, 0.0);
  const Rotation rotQuarterZ = Rotation(M_PI/2.0, 0.0, 0.0, 1.0);
  const Rotation rotIdentity = Rotation(0.0, 1.0, 0.0, 0.0);
  const Rotation rotGeneric = Rotation(2.0, typename Rotation::Vector3(3.0, 4.0, 5.0).normalized());

  Rotation rot;

  void assertNear(const Rotation_& rotA, const Rotation_& rotB, double tol=1e-6, const std::string& msg = "") {
    ASSERT_NEAR(rotA.angle(), rotB.angle(), tol) << msg;
    ASSERT_NEAR(rotA.axis().x(), rotB.axis().x(), tol) << msg;
    ASSERT_NEAR(rotA.axis().y(), rotB.axis().y(), tol) << msg;
    ASSERT_NEAR(rotA.axis().z(), rotB.axis().z(), tol) << msg;
  }
};


template <typename Rotation_>
struct RotationMatrixTestType {
  typedef Rotation_ Rotation;
  typedef typename Rotation::Scalar Scalar;

  Rotation rotQuarterX;
  Rotation rotQuarterY;
  Rotation rotQuarterZ;
  Rotation rotIdentity;
  Rotation rotGeneric;

  Rotation rot;

  void assertNear(const Rotation_& rotA, const Rotation_& rotB, double tol=1e-4, const std::string& msg = "") {
    KINDR_ASSERT_DOUBLE_MX_EQ(rotA.toStoredImplementation(), rotB.toStoredImplementation(), tol, msg);
  }

  RotationMatrixTestType() {
    if (Rotation_::Usage == kindr::rotations::RotationUsage::PASSIVE) {
      rotQuarterX = Rotation( 1.0,  0.0,  0.0,
                              0.0,  0.0,  1.0,
                              0.0, -1.0,  0.0); // psi=0, theta=0, phi=pi/2

      rotQuarterY = Rotation( 0.0,  0.0,  -1.0,
                            0.0,  1.0,  0.0,
                            1.0,  0.0,  0.0); // psi=0, theta=pi/2, phi=0

      rotQuarterZ = Rotation( 0.0,  1.0,  0.0,
                              -1.0,  0.0,  0.0,
                               0.0,  0.0,  1.0); // psi=pi/2, theta=0, phi=0

    }
    else {
      rotQuarterX = Rotation( 1.0,  0.0,  0.0,
                              0.0,  0.0,  -1.0,
                              0.0, 1.0,  0.0); // psi=0, theta=0, phi=pi/2

      rotQuarterY = Rotation( 0.0,  0.0,  1.0,
                            0.0,  1.0,  0.0,
                           -1.0,  0.0,  0.0); // psi=0, theta=pi/2, phi=0

      rotQuarterZ = Rotation( 0.0,  -1.0,  0.0,
                              1.0,  0.0,  0.0,
                              0.0,  0.0,  1.0); // psi=pi/2, theta=0, phi=0
     }

    rotIdentity = Rotation( 1.0,  0.0,  0.0,
                            0.0,  1.0,  0.0,
                            0.0,  0.0,  1.0);

    rotGeneric = Rotation(879.923176281257e-003,    372.025551942260e-003,   -295.520206661340e-003,
                           -327.579672728226e-003,    925.564159446682e-003,    189.796060978687e-003,
                            344.131896020075e-003,   -70.1995402393384e-003,    936.293363584199e-003); //psi=0.4, theta=0.3 phi=0.2

  }
};


typedef ::testing::Types<
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPF>, RotationVectorTestType<rot::RotationVectorPF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPF>, RotationVectorTestType<rot::RotationVectorPD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPD>, RotationVectorTestType<rot::RotationVectorPF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPD>, RotationVectorTestType<rot::RotationVectorPD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAF>, RotationVectorTestType<rot::RotationVectorAF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAF>, RotationVectorTestType<rot::RotationVectorAD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAD>, RotationVectorTestType<rot::RotationVectorAF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAD>, RotationVectorTestType<rot::RotationVectorAD>>,

    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPF>, RotationMatrixTestType<rot::RotationMatrixPF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPF>, RotationMatrixTestType<rot::RotationMatrixPD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPD>, RotationMatrixTestType<rot::RotationMatrixPF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPD>, RotationMatrixTestType<rot::RotationMatrixPD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAF>, RotationMatrixTestType<rot::RotationMatrixAF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAF>, RotationMatrixTestType<rot::RotationMatrixAD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAD>, RotationMatrixTestType<rot::RotationMatrixAF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAD>, RotationMatrixTestType<rot::RotationMatrixAD>>,

    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPF>, AngleAxisTestType<rot::AngleAxisPF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPF>, AngleAxisTestType<rot::AngleAxisPD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPD>, AngleAxisTestType<rot::AngleAxisPF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPD>, AngleAxisTestType<rot::AngleAxisPD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAF>, AngleAxisTestType<rot::AngleAxisAF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAF>, AngleAxisTestType<rot::AngleAxisAD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAD>, AngleAxisTestType<rot::AngleAxisAF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAD>, AngleAxisTestType<rot::AngleAxisAD>>,

    std::pair<RotationVectorTestType<rot::RotationVectorPF>, RotationMatrixTestType<rot::RotationMatrixPF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorPF>, RotationMatrixTestType<rot::RotationMatrixPD>>,
    std::pair<RotationVectorTestType<rot::RotationVectorPD>, RotationMatrixTestType<rot::RotationMatrixPF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorPD>, RotationMatrixTestType<rot::RotationMatrixPD>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAF>, RotationMatrixTestType<rot::RotationMatrixAF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAF>, RotationMatrixTestType<rot::RotationMatrixAD>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAD>, RotationMatrixTestType<rot::RotationMatrixAF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAD>, RotationMatrixTestType<rot::RotationMatrixAD>>,

    std::pair<RotationVectorTestType<rot::RotationVectorPF>, AngleAxisTestType<rot::AngleAxisPF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorPF>, AngleAxisTestType<rot::AngleAxisPD>>,
    std::pair<RotationVectorTestType<rot::RotationVectorPD>, AngleAxisTestType<rot::AngleAxisPF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorPD>, AngleAxisTestType<rot::AngleAxisPD>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAF>, AngleAxisTestType<rot::AngleAxisAF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAF>, AngleAxisTestType<rot::AngleAxisAD>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAD>, AngleAxisTestType<rot::AngleAxisAF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAD>, AngleAxisTestType<rot::AngleAxisAD>>,


    std::pair<RotationMatrixTestType<rot::RotationMatrixPF>, AngleAxisTestType<rot::AngleAxisPF>>,
    std::pair<RotationMatrixTestType<rot::RotationMatrixPF>, AngleAxisTestType<rot::AngleAxisPD>>,
    std::pair<RotationMatrixTestType<rot::RotationMatrixPD>, AngleAxisTestType<rot::AngleAxisPF>>,
    std::pair<RotationMatrixTestType<rot::RotationMatrixPD>, AngleAxisTestType<rot::AngleAxisPD>>,
    std::pair<RotationMatrixTestType<rot::RotationMatrixAF>, AngleAxisTestType<rot::AngleAxisAF>>,
    std::pair<RotationMatrixTestType<rot::RotationMatrixAF>, AngleAxisTestType<rot::AngleAxisAD>>,
    std::pair<RotationMatrixTestType<rot::RotationMatrixAD>, AngleAxisTestType<rot::AngleAxisAF>>,
    std::pair<RotationMatrixTestType<rot::RotationMatrixAD>, AngleAxisTestType<rot::AngleAxisAD>>

> TypeRotationPairs;


typedef ::testing::Types<
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPF>, RotationVectorTestType<rot::RotationVectorPF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPD>, RotationVectorTestType<rot::RotationVectorPD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAF>, RotationVectorTestType<rot::RotationVectorAF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAD>, RotationVectorTestType<rot::RotationVectorAD>>,

    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPF>, RotationMatrixTestType<rot::RotationMatrixPF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPD>, RotationMatrixTestType<rot::RotationMatrixPD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAF>, RotationMatrixTestType<rot::RotationMatrixAF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAD>, RotationMatrixTestType<rot::RotationMatrixAD>>,

    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPF>, AngleAxisTestType<rot::AngleAxisPF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionPD>, AngleAxisTestType<rot::AngleAxisPD>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAF>, AngleAxisTestType<rot::AngleAxisAF>>,
    std::pair<RotationQuaternionTestType<rot::RotationQuaternionAD>, AngleAxisTestType<rot::AngleAxisAD>>,

    std::pair<RotationVectorTestType<rot::RotationVectorPF>, RotationMatrixTestType<rot::RotationMatrixPF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorPD>, RotationMatrixTestType<rot::RotationMatrixPD>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAF>, RotationMatrixTestType<rot::RotationMatrixAF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAD>, RotationMatrixTestType<rot::RotationMatrixAD>>,

    std::pair<RotationVectorTestType<rot::RotationVectorPF>, AngleAxisTestType<rot::AngleAxisPF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorPD>, AngleAxisTestType<rot::AngleAxisPD>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAF>, AngleAxisTestType<rot::AngleAxisAF>>,
    std::pair<RotationVectorTestType<rot::RotationVectorAD>, AngleAxisTestType<rot::AngleAxisAD>>,


    std::pair<RotationMatrixTestType<rot::RotationMatrixPF>, AngleAxisTestType<rot::AngleAxisPF>>,
    std::pair<RotationMatrixTestType<rot::RotationMatrixPD>, AngleAxisTestType<rot::AngleAxisPD>>,
    std::pair<RotationMatrixTestType<rot::RotationMatrixAF>, AngleAxisTestType<rot::AngleAxisAF>>,
    std::pair<RotationMatrixTestType<rot::RotationMatrixAD>, AngleAxisTestType<rot::AngleAxisAD>>

> TypeRotationPrimTypePairs;



TYPED_TEST_CASE(ConversionTest, TypeRotationPairs);
TYPED_TEST_CASE(ConcatenationTest, TypeRotationPrimTypePairs);

TYPED_TEST(ConversionTest, testAToB) {
  /* vice versa */
  this->rotB.rot = this->rotA.rotIdentity;
  this->rotB.assertNear(this->rotB.rotIdentity, this->rotB.rot, this->tol, "Identity");

  this->rotB.rot = this->rotA.rotQuarterX;
  this->rotB.assertNear(this->rotB.rotQuarterX, this->rotB.rot, this->tol, "QuarterX");

  this->rotB.rot = this->rotA.rotQuarterY;
  this->rotB.assertNear(this->rotB.rotQuarterY, this->rotB.rot, this->tol, "QuarterY");

  this->rotB.rot = this->rotA.rotQuarterZ;
  this->rotB.assertNear(this->rotB.rotQuarterZ, this->rotB.rot, this->tol, "QuarterZ");

}



TYPED_TEST(ConversionTest, testBToA) {
  this->rotA.rot = this->rotB.rotIdentity;
  this->rotA.assertNear(this->rotA.rotIdentity, this->rotA.rot, this->tol, "Identity");

  this->rotA.rot = this->rotB.rotQuarterX;
  this->rotA.assertNear(this->rotA.rotQuarterX, this->rotA.rot, this->tol, "QuarterX");

  this->rotA.rot = this->rotB.rotQuarterY;
  this->rotA.assertNear(this->rotA.rotQuarterY, this->rotA.rot, this->tol, "QuarterY");

  this->rotA.rot = this->rotB.rotQuarterZ;
  this->rotA.assertNear(this->rotA.rotQuarterZ, this->rotA.rot, this->tol, "QuarterZ");
}

TYPED_TEST(ConcatenationTest, testAToB) {

  // Check result of multiplication of a generic rotation with identity
  this->rotB.rot = this->rotB.rotGeneric*this->rotA.rotIdentity;
  this->rotB.assertNear(this->rotB.rotGeneric.getUnique(), this->rotB.rot.getUnique(), this->tol, "rhs: identity");

  this->rotB.rot = this->rotA.rotIdentity*this->rotB.rotGeneric;
  this->rotB.assertNear(this->rotB.rotGeneric.getUnique(), this->rotB.rot.getUnique(), this->tol, "lhs: identity");

  // Check concatenation of 4 quarters
  this->rotB.rot = this->rotA.rotQuarterX*this->rotB.rotQuarterX*this->rotA.rotQuarterX*this->rotB.rotQuarterX;
  this->rotB.assertNear(this->rotB.rotIdentity, this->rotB.rot.getUnique(), this->tol, "4 quarters");

  this->rotB.rot = this->rotA.rotQuarterY*this->rotB.rotQuarterY*this->rotA.rotQuarterY*this->rotB.rotQuarterY;
  this->rotB.assertNear(this->rotB.rotIdentity, this->rotB.rot.getUnique(), this->tol, "4 quarters");

  this->rotB.rot = this->rotA.rotQuarterZ*this->rotB.rotQuarterZ*this->rotA.rotQuarterZ*this->rotB.rotQuarterZ;
  this->rotB.assertNear(this->rotB.rotIdentity, this->rotB.rot.getUnique(), this->tol, "4 quarters");

  // check concatenation of 3 different quarters
  this->rotB.rot = this->rotB.rotQuarterX.inverted()*this->rotA.rotQuarterY*this->rotB.rotQuarterX;
  if(this->rotB.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotB.rot.invert();
  }
  this->rotB.assertNear(this->rotB.rotQuarterZ, this->rotB.rot.getUnique(), this->tol, "concatenation 1");

  this->rotB.rot = this->rotB.rotQuarterX.inverted()*this->rotA.rotQuarterZ*this->rotB.rotQuarterX;
  if(this->rotB.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotB.rot.invert();
  }
  this->rotB.assertNear(this->rotB.rotQuarterY.inverted(), this->rotB.rot.getUnique(), this->tol, "concatenation 2");

  this->rotB.rot = this->rotB.rotQuarterY.inverted()*this->rotA.rotQuarterX*this->rotB.rotQuarterY;
  if(this->rotB.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotB.rot.invert();
  }
  this->rotB.assertNear(this->rotB.rotQuarterZ.inverted(), this->rotB.rot.getUnique(), this->tol, "concatenation 3");

  this->rotB.rot = this->rotB.rotQuarterY.inverted()*this->rotA.rotQuarterZ*this->rotB.rotQuarterY;
  if(this->rotB.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotB.rot.invert();
  }
  this->rotB.assertNear(this->rotB.rotQuarterX, this->rotB.rot.getUnique(), this->tol, "concatenation 4");

  this->rotB.rot = this->rotB.rotQuarterZ.inverted()*this->rotA.rotQuarterX*this->rotB.rotQuarterZ;
  if(this->rotB.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotB.rot.invert();
  }
  this->rotB.assertNear(this->rotB.rotQuarterY, this->rotB.rot.getUnique(), this->tol, "concatenation 5");

  this->rotB.rot = this->rotB.rotQuarterZ.inverted()*this->rotA.rotQuarterY*this->rotB.rotQuarterZ;
  if(this->rotB.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotB.rot.invert();
  }
  this->rotB.assertNear(this->rotB.rotQuarterX.inverted(), this->rotB.rot.getUnique(), this->tol, "concatenation 6");
}



TYPED_TEST(ConcatenationTest, testBToA) {

  // Check result of multiplication of a generic rotation with identity
  this->rotA.rot = this->rotA.rotGeneric*this->rotB.rotIdentity;
  this->rotA.assertNear(this->rotA.rotGeneric.getUnique(), this->rotA.rot.getUnique(), this->tol, "rhs: identity");

  this->rotA.rot = this->rotB.rotIdentity*this->rotA.rotGeneric;
  this->rotA.assertNear(this->rotA.rotGeneric.getUnique(), this->rotA.rot.getUnique(), this->tol, "lhs: identity");

  // Check concatenation of 4 quarters
  this->rotA.rot = this->rotB.rotQuarterX*this->rotA.rotQuarterX*this->rotB.rotQuarterX*this->rotA.rotQuarterX;
  this->rotA.assertNear(this->rotA.rotIdentity, this->rotA.rot.getUnique(), this->tol, "4 quarters");

  this->rotA.rot = this->rotB.rotQuarterY*this->rotA.rotQuarterY*this->rotB.rotQuarterY*this->rotA.rotQuarterY;
  this->rotA.assertNear(this->rotA.rotIdentity, this->rotA.rot.getUnique(), this->tol, "4 quarters");

  this->rotA.rot = this->rotB.rotQuarterZ*this->rotA.rotQuarterZ*this->rotB.rotQuarterZ*this->rotA.rotQuarterZ;
  this->rotA.assertNear(this->rotA.rotIdentity, this->rotA.rot.getUnique(), this->tol, "4 quarters");


  // Check concatenation of 3 different quarters
  this->rotA.rot = this->rotA.rotQuarterX.inverted()*this->rotB.rotQuarterY*this->rotA.rotQuarterX;
  if(this->rotA.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotA.rot.invert();
  }
  this->rotA.assertNear(this->rotA.rotQuarterZ, this->rotA.rot.getUnique(), this->tol, "concatenation 1");

  this->rotA.rot = this->rotA.rotQuarterX.inverted()*this->rotB.rotQuarterZ*this->rotA.rotQuarterX;
  if(this->rotA.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotA.rot.invert();
  }
  this->rotA.assertNear(this->rotA.rotQuarterY.inverted(), this->rotA.rot.getUnique(), this->tol, "concatenation 2");

  this->rotA.rot = this->rotA.rotQuarterY.inverted()*this->rotB.rotQuarterX*this->rotA.rotQuarterY;
  if(this->rotA.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotA.rot.invert();
  }
  this->rotA.assertNear(this->rotA.rotQuarterZ.inverted(), this->rotA.rot.getUnique(), this->tol, "concatenation 3");

  this->rotA.rot = this->rotA.rotQuarterY.inverted()*this->rotB.rotQuarterZ*this->rotA.rotQuarterY;
  if(this->rotA.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotA.rot.invert();
  }
  this->rotA.assertNear(this->rotA.rotQuarterX, this->rotA.rot.getUnique(), this->tol, "concatenation 4");

  this->rotA.rot = this->rotA.rotQuarterZ.inverted()*this->rotB.rotQuarterX*this->rotA.rotQuarterZ;
  if(this->rotA.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotA.rot.invert();
  }
  this->rotA.assertNear(this->rotA.rotQuarterY, this->rotA.rot.getUnique(), this->tol, "concatenation 5");

  this->rotA.rot = this->rotA.rotQuarterZ.inverted()*this->rotB.rotQuarterY*this->rotA.rotQuarterZ;
  if(this->rotA.rot.Usage == kindr::rotations::RotationUsage::ACTIVE){
    this->rotA.rot.invert();
  }
  this->rotA.assertNear(this->rotA.rotQuarterX.inverted(), this->rotA.rot.getUnique(), this->tol, "concatenation 6");


}






