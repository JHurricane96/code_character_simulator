/**
 * @file projectile_handler.h
 *
 * Contains the projectile handler class
 */

#ifndef STATE_ACTOR_PROJECTILE_HANDLER_H
#define STATE_ACTOR_PROJECTILE_HANDLER_H

#include "actor/actor.h"
#include "actor/archer.h"
#include "actor/arrow.h"
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
	 * A vector of arrows that the Projectile Handler is handling at present
	 */
	std::vector<std::shared_ptr<Arrow> > arrows;
	/**
	 * Holds the value of the next arrow's actor id
	 */
	int64_t next_arrow_id;
	/**
	 * A helper function to handle Actor updates
	 *
	 * Iterates through towers and creates arrows if the Tower has an
	 * attack target set
	 *
	 * @param      towers   The towers
	 * @param      terrain  The terrain
	 *
	 * @tparam     ActorType  Either Tower or Archer
	 */
	template <typename ActorType>
	void HandleActors(
		std::vector<std::vector<std::shared_ptr<ActorType> > >& actors,
		Terrain* terrain);
	/**
	 * A helper function to handle Arrow updates
	 *
	 * Calls the arrow's update function and removes an arrow if it has
	 * reached destination
	 *
	 * @param[in]  delta_time  The delta time
	 */
	void HandleArrows(int64_t delta_time);

public:
	ProjectileHandler();
	/**
	 * Constructor for the Projectile Handler
	 *
	 * @param[in]  first_arrow_id  The ID for the first arrow to be
	 * 							   instantiated
	 */
	ProjectileHandler(int64_t first_arrow_id);
	/**
	 * Getter for the arrows
	 *
	 * @return     The arrows
	 */
	std::vector<std::shared_ptr<Arrow> > GetProjectiles();
	/**
	 * Creates an arrow to be fired
	 *
	 * @param[in]  player_id      The ID of the player who fires
	 * @param[in]  start          The start position
	 * @param      attack_target  The actor to attack
	 * @param[in]  max_speed      The maximum speed of the arrow
	 * @param[in]  ttl            The life time of the arrow (in clock ticks)
	 * @param[in]  size            The size of the arrow
	 * @param[in]  damage         The damage the arrow can cause
	 */
	void CreateArrow(
		PlayerId player_id,
		physics::Vector2D start,
		Actor* attack_target,
		int64_t max_speed,
		int64_t ttl,
		int64_t size,
		int64_t damage
	);
	/**
	 * Handles the arrow states
	 *
	 * Cycles through actors and creates arrows if the Archer or Tower has a
	 * attack target set
	 *
	 * Updates the arrow position if the arrow hasn't reached its destination
	 * yet; and if it has reached, inflicts damage on the target
	 *
	 * @param[in]  delta_time  The delta time
	 * @param      towers      vector of towers
	 * @param      archers     vector of archers
	 * @param      terrain     The terrain
	 */
	void Update(
		int64_t delta_time,
		std::vector<std::vector<std::shared_ptr<Tower> > >& towers,
		std::vector<std::vector<std::shared_ptr<Archer> > >& archers,
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
