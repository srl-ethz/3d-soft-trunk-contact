#ifndef ARMPCC_H
#define ARMPCC_H

#define USE_ROS false  // do you want to publish joint states to ROS

#include <rbdl/rbdl.h>
#include "SoftTrunk_common_defs.h"

#ifndef RBDL_BUILD_ADDON_URDFREADER
	#error "Error: RBDL addon URDFReader not enabled."
#endif

#include <rbdl/addons/urdfreader/urdfreader.h>

#include <iostream>
#include <fstream>

using namespace RigidBodyDynamics;
using namespace RigidBodyDynamics::Math;

/**
 * @brief Represents the augmented rigid arm model.
 * @details The rigid arm model  approximates the soft arm. (see paper etc. for more info on how this is so)
 * This class can calculate the kinematics & dynamics of the augmented arm using RBDL.
 * It can also generate XACRO files based on the parameters of the robot.
 */
class AugmentedRigidArm{
private:
  Model* rbdl_model;
  /**
   * @brief create a RBDL model and save it to member variable rbdl_model
   */
  void create_rbdl_model();
  /**
   * @brief publish joint state to ROS
   */
  void joint_publish();
  /**
   * @brief extract inertia matrix(B) and gravity vector(G) of the current arm configuration(xi).
   */
  void extract_B_G();
  /**
   * @brief generate a file robot.urdf.xacro, using the lengths and masses of the actual robot.
   */
  void create_xacro();
public:
    /**
     * @param is_create_xacro set to true if you only want to generate the model's xacro model
     */
  explicit AugmentedRigidArm(bool is_create_xacro=false);
  /**
   * @brief length of each segment, in meters
   */
  std::vector<double> lengths;
  /**
   * @brief mass of each segment, in kg
   */
  std::vector<double> masses;
  Eigen::Matrix<double, NUM_ELEMENTS*6, 1> xi;
  /**
   * @brief the Jacobian that maps from q to xi
   */
  Eigen::Matrix<double, NUM_ELEMENTS*6, NUM_ELEMENTS*2> Jxi=Eigen::Matrix<double, NUM_ELEMENTS*6, NUM_ELEMENTS*2>::Zero(); // Jacobian
  /**
   * @brief the time derivative of the Jacobian that maps from q to xi
   */
  Eigen::Matrix<double, NUM_ELEMENTS*6, NUM_ELEMENTS*2> dJxi=Eigen::Matrix<double, NUM_ELEMENTS*6, NUM_ELEMENTS*2>::Zero(); // time derivative of Jacobian
  Eigen::Matrix<double, 3, NUM_ELEMENTS*2> update_J(Vector2Nd q); // used for inverse kinematics

  /**
   * @brief inertia matrix
   */
  Eigen::Matrix<double, NUM_ELEMENTS*6, NUM_ELEMENTS*6> B_xi;
  /**
   * @brief the gravity vector, i.e. the force at each joint when the arm is completely stationary at its current configuration.
   */
  Eigen::Matrix<double, NUM_ELEMENTS*6, 1> G_xi;
  /**
   * @brief update the member variables based on current values
   */
  void update(Vector2Nd, Vector2Nd);
  void update_xi(Vector2Nd);
  void update_Jxi(Vector2Nd);
  void update_dJxi(Vector2Nd, Vector2Nd);
};
#endif
