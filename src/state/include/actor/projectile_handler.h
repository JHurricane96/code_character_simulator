/**
 * @file projectile_handler.h
 *
 * Contains the projectile handler class
 */

#ifndef STATE_ACTOR_PROJECTILE_HANDLER_H
#define STATE_ACTOR_PROJECTILE_HANDLER_H

#include "actor/actor.h"
#include "actor/magician.h"
#include "actor/fire_ball.h"
#include "actor/tower.h"
#include "state_export.h"
#include "terrain/terrain.h"
#include "utilities.h"
#include "vector2d.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace state {
/**
 * Class for handling all projectiles
 */
class STATE_EXPORT ProjectileHandler {
private:
	/**
	 * A vector of fire_balls that the Projectile Handler is handling at present
	 */
	std::vector<std::shared_ptr<FireBall> > fire_balls;
	/**
	 * Holds the value of the next fire_ball's actor id
	 */
	int64_t next_fire_ball_id;
	/**
	 * A helper function to handle Actor updates
	 *
	 * Iterates through towers and creates fire_balls if the Tower has an
	 * attack target set
	 *
	 * @param      towers   The towers
	 * @param      terrain  The terrain
	 *
	 * @tparam     ActorType  Either Tower or Magician
	 */
	template <typename ActorType>
	void HandleActors(
		std::vector<std::vector<std::shared_ptr<ActorType> > >& actors,
		Terrain* terrain);
	/**
	 * A helper function to handle FireBall updates
	 *
	 * Calls the fire_ball's update function and removes a fire_ball if it has
	 * reached destination
	 *
	 * @param[in]  delta_time  The delta time
	 */
	void HandleFireBalls(float delta_time);

public:
	ProjectileHandler();
	/**
	 * Constructor for the Projectile Handler
	 *
	 * @param[in]  first_fire_ball_id  The ID for the first fire_ball to be
	 *                                 instantiated
	 */
	ProjectileHandler(int64_t first_fire_ball_id);
	/**
	 * Getter for the fire_balls
	 *
	 * @return     The fire_balls
	 */
	std::vector<std::shared_ptr<FireBall> > GetProjectiles();
	/**
	 * Creates a fire_ball to be fired
	 *
	 * @param[in]  player_id      The ID of the player who fires
	 * @param[in]  start          The start position
	 * @param      attack_target  The actor to attack
	 * @param[in]  max_speed      The maximum speed of the fire_ball
	 * @param[in]  ttl            The life time of the fire_ball (in clock ticks)
	 * @param[in]  size           The size of the fire_ball
	 * @param[in]  damage         The damage the fire_ball can cause
	 */
	void CreateFireBall(
		PlayerId player_id,
		physics::Vector2D start,
		Actor* attack_target,
		int64_t max_speed,
		int64_t ttl,
		int64_t size,
		int64_t damage
	);
	/**
	 * Handles the fire_ball states
	 *
	 * Cycles through actors and creates fire_balls if the Magician or Tower has a
	 * attack target set
	 *
	 * Updates the fire_ball position if the fire_ball hasn't reached its
	 * destination yet; and if it has reached, inflicts damage on the target
	 *
	 * @param[in]  delta_time  The delta time
	 * @param      towers      vector of towers
	 * @param      magicians   vector of magicians
	 * @param      terrain     The terrain
	 */
	void Update(
		float delta_time,
		std::vector<std::vector<std::shared_ptr<Tower> > >& towers,
		std::vector<std::vector<std::shared_ptr<Magician> > >& magicians,
		Terrain* terrain
	);
	/**
	 * Merges this, a player state's ProjectileHandler, with the main
	 * state's ProjectileHandler
	 *
	 * @param[in]  proj_handler  The player's ProjectileHandler
	 * @param[in]  actors        The player's state's Actors. Any
	 *                           references to the other state's Actors
	 *                           are replaced by this state's Actors
	 */
	void MergeWithMain(
		const ProjectileHandler& proj_handler,
		std::vector<std::shared_ptr<Actor> > actors
	);
};
}

#endif
