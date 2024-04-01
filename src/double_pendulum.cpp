#include "double_pendulum.hpp"
#include "constants.hpp"
#include <cmath>

using Constants::g;

/*
Description:
  Create an instance on a double pendulum.
Arguments:
  glm::vec2 masses: A vector containing the masses in kg
                    of the two pendulums.
  glm::vec2 lengths: A vector containing the lengths in meters
                     of the rods where the masses are
                     attached
  glm::vec2 start_angles: The initial angles of the
                          two pendulums. The angles are
                          in radians, the zero is set on
                          the vertical, under the pendulum.
                          The angles get positive in 
                          anticlockwise direction.
  glm::vec2 start_velocities: The initial velocities of
                              the pendulums in rad/s.
                              Anticlockwise is positive.
*/

DoublePendulum::DoublePendulum(glm::vec2 masses, glm::vec2 lengths, glm::vec2 start_angles, glm::vec2 start_velocities)
                 : masses(masses), lengths(lengths), angles(start_angles), ang_velocities(start_velocities) {};

/*
Description:
  Update the pendulum position and velocity using
  a formula found with the lagrangian of the
  double pendulum.
  Does the formula work? I have no idea, we'll see
  if i implement the graphics things.

Arguments:
  double deltaTime: the amount of time in seconds
                    passed since the last update.
*/
void DoublePendulum::update(double deltaTime) {
  double a = (masses[0] + masses[1]) * std::pow(lengths[0], 2.0);
  double b = masses[1] * lengths[0] * lengths[1] * std::cos(angles[0] - angles[1]);
  double c = masses[1] * lengths[0] * lengths[1] * ang_velocities[0] * ang_velocities[1] * std::sin(angles[0] - angles[1])
             + (masses[0] + masses[1]) * g * lengths[0] * std::sin(angles[0])
             + masses[1] * lengths[0] * lengths[1] * ang_velocities[1] * (ang_velocities[0] - ang_velocities[1]) * std::sin(angles[0] - angles[1]);
  double ap = masses[1] * lengths[0] * lengths[1] * std::cos(angles[0] - angles[1]);
  double bp = masses[1] * std::pow(lengths[1], 2.0);
  double cp = masses[1] * lengths[0] * lengths[1] * ang_velocities[0] * ang_velocities[1] * std::sin(angles[0] - angles[1])
              + masses[1] * g * lengths[1] * std::sin(angles[1])
              + masses[1] * lengths[0] * lengths[1] * ang_velocities[0] * (ang_velocities[0] - ang_velocities[1]) * std::sin(angles[0] - angles[1]);

  ang_velocities[0] += deltaTime * (bp*c - b*cp)/(a*bp - ap*b);
  angles[0] += deltaTime * ang_velocities[0];

  ang_velocities[1] += deltaTime * (a*cp - ap*c)/(a*bp - ap*b);
  angles[1] += deltaTime * ang_velocities[1];
}

/*
Description:
  Set new angles to the pendulums.
Arguments:
  glm::vec2 new_angles: The new angles of the
                        pendulums, in radians, 
                        the zero is set on the
                        vertical, under the pendulum.
                        The angles get positive in 
                        anticlockwise direction. 
*/
void DoublePendulum::setAngles(glm::vec2 new_angles) {
  angles = new_angles;
}

/*
Description:
  Get the pendulums' angles.
Return value:
  The angles of the
  pendulums, in radians, 
  the zero is set on the
  vertical, under the pendulum.
  The angles get positive in 
  anticlockwise direction. 
*/
glm::vec2 DoublePendulum::getAngles() const {
  return angles;
}
