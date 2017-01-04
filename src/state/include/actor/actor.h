/**
 * @file actor.h
 * Defines the Base Actor class that all actors derive from
 */

#ifndef STATE_ACTOR_ACTOR_H
#define STATE_ACTOR_ACTOR_H

#include <cstdint>
#include <memory>
#include "actor/actor.fwd.h"
#include "vector2d.h"
#include "actor/states/actor_state.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"
#include "utilities.h"

namespace state {

/**
 * The various types of Actors that exist
 */
enum class ActorType {
	ARCHER,
	ARROW,
	BASE,
	FLAG,
	KING,
	SCOUT,
	SWORDSMAN,
	TOWER,
};

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
	 * The current state of the actor
	 */
	std::unique_ptr<ActorState> state;
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
	Actor * attack_target;
	/**
	 * The radius of the actor's LOS
	 */
	int64_t los_radius;
	/**
	 * The Actor's attack speed
	 *
	 * The total time it takes to execute an attack,
	 * i.e., the charge-up time for an attack
	 */
	int64_t attack_speed;
	/**
	 * If true, actor is dead, false otherwise
	 */
	bool is_dead;
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
		int64_t los_radius,
		int64_t attack_speed
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
	 * Gets the Actor's attack, the amount of damage it can deal
	 *
	 * @return     The Actor's attack
	 */
	int64_t GetAttack();
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
	 * Gets the Actor's attack_target
	 *
	 * @return     The Actor's attack_target
	 */
	Actor * GetAttackTarget();
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
	 * Gets the Actor's attack_speed
	 *
	 * @return     The attack_speed
	 */
	int64_t GetAttackSpeed();
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
	 * Determines if the Actor is dead
	 *
	 * @return     true if dead, false otherwises
	 */
	bool IsDead();
	/**
	 * Makes the Actor die
	 *
	 * Dead Actors can't do anything
	 *
	 * Sets is_dead to true
	 * Sets time_to_respawn to total_respawn_time
	 * Sets hp to max_hp
	 * Sets time_spent_near_base to 0
	 * Sets position to (0, 0)
	 * Sets attack_target to nullptr
	 */
	void Die();
	/**
	 * Set's the actor's respawn time to 0
	 */
	void Respawn();
	/**
	 * Executes the Actor's state machine
	 *
	 * Changes Actor's state if necessary
	 *
	 * Actor's state calls necessary methods in Actor
	 *
	 * Meant to be called in Actor::Update
	 *
	 * @param[in]  delta_time  The difference in time between the
	 *                         previous and current Update calls
	 */
	void DecideState(int64_t delta_time);
	/**
	 * Start attacking a target
	 *
	 * Sets attack_target to the given target
	 *
	 * @param      target  The attack target
	 */
	void AttackUnit(Actor * target);
	/**
	 * Stops the Actor from attacking if he was attacking
	 *
	 * Sets attack_target to nullptr
	 */
	void StopAttack();
	/**
	 * Sets Actor's hp to hp - damage_amount
	 *
	 * @param[in]  damage_amount  The damage amount
	 */
	void Damage(int64_t damage_amount);

};

}

#endif
