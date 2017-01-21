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
	 * The type of this Actor
	 */
	ActorType actor_type;
	/**
	 * The current state of the actor
	 */
	std::unique_ptr<ActorState> state;
	/**
	 * The damage the actor can deal
	 */
	int64_t attack;
	/**
	 * The number of health points the actor currently has
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
	float time_to_respawn;
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
	 * The range within which the actor can attack
	 */
	int64_t attack_range;
	/**
	 * If true, actor is dead, false otherwise
	 */
	bool is_dead;
	/**
	 * A player sets this for a dead Actor to signal that he's ready to
	 * respawn it, assuming time_to_respawn is 0
	 *
	 * Set to (-1, -1) on death
	 */
	physics::Vector2D respawn_location;
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
		float time_to_respawn,
		int64_t time_spent_near_base,
		physics::Vector2D position,
		physics::Vector2D velocity,
		int64_t los_radius,
		int64_t attack_speed,
		int64_t attack_range,
		ActorType actor_type
	);
	Actor(const Actor& other);
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
	virtual void Update(float delta_time) = 0;
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
	 * Gets the Actor's type, i.e., actor_type
	 *
	 * @return     The Actor's type
	 */
	ActorType GetActorType();
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
	 * Gets the Actor's attack range
	 *
	 * @return     The attack_range
	 */
	int64_t GetAttackRange();
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
	 * A virtual method for units to override based on their attack style
	 */
	virtual void Attack();
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
	 * Default behaviour:
	 * Sets is_dead to true
	 * Sets time_to_respawn to total_respawn_time
	 * Sets hp to 0
	 * Sets time_spent_near_base to 0
	 * Sets position to (0, 0)
	 * Sets respawn_location to (-1, -1)
	 * Sets attack_target to nullptr
	 */
	virtual void Die();
	/**
	 * Respawns the Actor
	 *
	 * This is meant to be called after a player sets this Actor's
	 * respawn location after its time_to_respawn hits 0
	 *
	 * Sets is_dead to false
	 * Sets hp to max_hp
	 * Sets position to respawn_location
	 */
	void Respawn();
	/**
	 * Decrease time_to_respawn
	 *
	 * Used when the Actor is dead and is called every tick
	 *
	 * @param[in]  delta_time  The amount to decrease by
	 */
	void DecreaseRespawnTime(float delta_time);
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
	void DecideState(float delta_time);
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
	 * Actors can inherit to define how to stop attack
	 *
	 * Default behaviour is to set attack_target to nullptr
	 */
	virtual void StopAttack();
	/**
	 * Sets Actor's hp to hp - damage_amount
	 *
	 * @param[in]  damage_amount  The damage amount
	 */
	void Damage(int64_t damage_amount);
	/**
	 * Gets the Actor's respawn location
	 *
	 * @return     The Actor's respawn location
	 */
	physics::Vector2D GetRespawnLocation();
	/**
	 * Sets the Actor's respawn location
	 *
	 * @param[in]  respawn_location  The new respawn location
	 */
	void SetRespawnLocation(physics::Vector2D respawn_location);
	/**
	 * Merges this, an Actor in the main state, with the corresponding
	 * Actor in a player's state
	 *
	 * @param[in]  actor   The player's Actor
	 * @param[in]  actors  The main state's Actors. Any references
	 *                     to the other state's Actors are replaced
	 *                     by this state's Actors
	 */
	void MergeWithBuffer(
		const Actor * actor,
		std::vector<std::shared_ptr<Actor> > actors
	);
	/**
	 * Checks if this Actor is within the given bounds
	 *
	 * The bounds are x from 0 to bounds.x and y from 0 to bounds.y.
	 * If not within bounds, sets the Actor's position to be within
	 * them
	 *
	 * @param[in]  bounds  The bounds
	 */
	void CheckBounds(physics::Vector2D bounds);
	/**
	 * Merges this, an Actor in a player's state, with the
	 * corresponding Actor in the main state
	 *
	 * @param[in]  actor   The player's Actor
	 * @param[in]  actors  The player's state's Actors. Any
	 *                     references to the other state's Actors
	 *                     are replaced by this state's Actors
	 */
	void MergeWithMain(
		const Actor * actor,
		std::vector<std::shared_ptr<Actor> > actors
	);
};

}

#endif
