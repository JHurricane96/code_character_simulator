/**
 * @file actor.h
 * Defines the Base Actor class that all actors derive from
 */

#ifndef STATE_ACTOR_ACTOR_H
#define STATE_ACTOR_ACTOR_H

#include <cstdint>
#include "actor/actor.fwd.h"
#include "vector2d.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"
#include "utilities.h"

namespace state {

/**
 * The base Actor Class
 */
class STATE_EXPORT Actor {
protected:
	/**
	 * The actor ID
	 */
	act_id_t id;
	/**
	 * ID of the player that owns this actor
	 */
	PlayerId player_id;
	/**
	 * The damage the actor can deal
	 */
	int64_t attack;
	/**
	 * The number of helath points the actor currently has
	 */
	int64_t hp;
	/**
	 * The max HP of the actor
	 */
	int64_t max_hp;
	/**
	 * The max speed the actor can travel at
	 */
	int64_t max_speed;
	/**
	 * Radius of the actor
	 *
	 * Assumes actors are circles
	 */
	int64_t size;
	/**
	 * Total time taken to respawn at the base if dead
	 */
	int64_t total_respawn_time;
	/**
	 * The respawn time remaining (0 if alive)
	 */
	int64_t time_to_respawn;
	/**
	 * For base poisoning calculation
	 * If the actor spends too much time near the base, he starts to lose hp
	 * To discourage overly defensive formations
	 */
	int64_t time_spent_near_base;
	/**
	 * The actor's position vector
	 */
	physics::Vector2D position;
	/**
	 * The actor's velocity vector
	 */
	physics::Vector2D velocity;
	/**
	 * The actor's attack target if it is given one
	 */
	act_id_t attack_target_id;
	/**
	 * The radius of the actor's LOS
	 */
	int64_t los_radius;
	/**
	 * An instance of the PathPlannerHelper class to help with path planning
	 */
	PathPlannerHelper path_planner_helper;
public:
	/**
	 * Constructor for Actor class
	 */
	Actor();

	Actor(
		act_id_t id,
		PlayerId player_id,
		int64_t attack,
		int64_t hp,
		int64_t max_hp,
		int64_t max_speed,
		int64_t size,
		int64_t total_respawn_time,
		int64_t time_to_respawn,
		int64_t time_spent_near_base,
		physics::Vector2D position,
		physics::Vector2D velocity,
		int64_t los_radius
	);
	void AddPathPlanner(PathPlannerHelper p);
	/**
	 * Gets the path planner helper
	 *
	 * @return     The path planner helper
	 */
	PathPlannerHelper* GetPathPlannerHelper();
	/**
	 * Update function to be called every tick
	 *
	 * @param[in]  delta_time  The difference in time between the
	 *                         previous and current Update calls
	 */
	virtual void Update(int64_t delta_time) = 0;
	/**
	 * Gets the Actor ID
	 *
	 * @return     The Actor ID
	 */
	act_id_t GetId();
	/**
	 * Gets the ID of the player that owns this Actor
	 *
	 * @return     The required player's ID
	 */
	PlayerId GetPlayerId();
	/**
	 * Gets the Actor's HP
	 *
	 * @return     The Actor's HP
	 */
	int64_t GetHp();
	/**
	 * Gets the Actor's maximum HP
	 *
	 * @return     The Actor's maximum HP
	 */
	int64_t GetMaxHp();
	/**
	 * Gets the Actor's maximum speed.
	 *
	 * @return     The Actor's maximum speed.
	 */
	int64_t GetMaxSpeed();
	/**
	 * Gets the Actor's size
	 *
	 * @return     The size
	 */
	int64_t GetSize();
	/**
	 * Gets the time to respawn.
	 *
	 * @return     The time to respawn.
	 */
	int64_t GetTimeToRespawn();
	/**
	 * Gets the Actor's attack target ID
	 *
	 * @return     The Actor's attack target ID
	 */
	act_id_t GetAttackTargetId();
	/**
	 * Gets the Actor's velocity vector
	 *
	 * @return     The Actor's velocity vector
	 */
	physics::Vector2D GetVelocity();
	/**
	 * Gets the los radius of the actor
	 *
	 * @return     The los radius
	 */
	int64_t GetLosRadius();
	/**
	 * Sets the Actor's velocity
	 *
	 * @param[in]  new_velocity  The new velocity
	 */
	void SetVelocity(physics::Vector2D new_velocity);
	/**
	 * Gets the Actor's position vector
	 *
	 * @return     The Actor's position vector
	 */
	physics::Vector2D GetPosition();
	/**
	 * Set's the actor's respawn time to 0
	 */
	void Respawn();
};

}

#endif
