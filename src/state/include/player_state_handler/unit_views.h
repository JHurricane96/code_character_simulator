/**
 * @file vnit_views.h
 *
 * Definitions for the restricted unit views for players
 */

#ifndef STATE_PLAYER_STATE_HANDLER_UNIT_VIEWS_H
#define STATE_PLAYER_STATE_HANDLER_UNIT_VIEWS_H

#include "actor/actor.h"
#include "actor/tower.h"
#include "actor/magician.h"
#include "vector2d.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"
#include "utilities.h"
#include <cstdint>

namespace state {

/**
 * Player view for PathPlannerHelper
 *
 * @see PathPlannerHelper
 */
class STATE_EXPORT PathPlannerHelperView {
private:
	bool is_path_planning;
public:
	explicit PathPlannerHelperView(PathPlannerHelper * path_planner_helper);
	/**
	 * Determines if the unit is path planning
	 *
	 * @return     True if path planning, False otherwise.
	 */
	bool IsPathPlanning();
};

/**
 * Provides a restricted view of the enemy units to the player
 * Given the enemy Actor ID, the player can view the position of the unit
 */
class STATE_EXPORT EnemyUnitView {
private:
	/**
	* The actor ID
	*/
	act_id_t id;
	/**
	* The type of this Actor
	*/
	ActorType actor_type;
	/**
	 * The HP of this Actor
	 */
	int64_t hp;
	/**
	 * Radius of the Actor
	 *
	 * Assumes Actors are circles
	 */
	int64_t size;
	/**
	* Position of the actor
	*/
	physics::Vector2D position;
public:
	EnemyUnitView();
	explicit EnemyUnitView(Actor * actor);
	/**
	 * Gets the ID of the enemy
	 *
	 * @return     The ID
	 */
	act_id_t GetId();
	/**
	 * Gets the position vector of the enemy
	 *
	 * @return     The position vector
	 */
	physics::Vector2D GetPosition();
	/**
	 * Gets the Actor's type
	 *
	 * @return     The Actor's type
	 */
	ActorType GetActorType();
	/**
	 * Gets the Actor's size
	 *
	 * @return     The Actor's size
	 */
	int64_t GetSize();
	/**
	 * Gets the Actor's HP
	 *
	 * @return     The Actor's HP
	 */
	int64_t GetHp();
};

/**
 * Player view for Actors that are movable.
 *
 * @see Actor
 */
class STATE_EXPORT UnitView {
private:
	/**
	 * The actor ID
	 */
	act_id_t id;
	/**
	 * The type of this Actor
	 */
	ActorType actor_type;
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
	 * The current speed the actor is travelling at
	 */
	float speed;
	/**
	 * Radius of the Actor
	 *
	 * Assumes Actors are circles
	 */
	int64_t size;
	/**
	 * The player's attack target
	 */
	std::unique_ptr<EnemyUnitView> attack_target;
	/**
	 * The actor's position vector
	 */
	physics::Vector2D position;
	/**
	 * The actor's velocity vector
	 */
	physics::Vector2D velocity;
	/**
	 * The radius of the actor's LOS
	 */
	int64_t los_radius;
	/**
	 * The range within which the actor can attack
	 */
	int64_t attack_range;
	/**
	 * The actor's path planner helper view
	 */
	PathPlannerHelperView path_planner_helper;
public:
	UnitView();
	explicit UnitView(Actor * actor);
	UnitView(const UnitView& other);
	/**
	 * Gets the ID of the actor
	 *
	 * @return     The ID
	 */
	act_id_t GetId();
	/**
	 * Gets the health of the actor
	 *
	 * @return     The hp
	 */
	int64_t GetHp();
	/**
	 * Gets the maximum health of the actor
	 *
	 * @return     The maximum health
	 */
	int64_t GetMaxHp();
	/**
	 * Gets the maximum speed possible for the actor
	 *
	 * @return     The maximum speed
	 */
	int64_t GetMaxSpeed();
	/**
	 * Gets the Actor's size
	 *
	 * @return     The Actor's size
	 */
	int64_t GetSize();
	/**
	 * Gets the Actor's type
	 *
	 * @return     The Actor's type
	 */
	ActorType GetActorType();
	/**
	 * Gets the actor's target
	 *
	 * success is:
	 * - 0  if the actor isn't attccking
	 * - 1  if the actor is attacking
	 *
	 * @param      success    If valid pointer, holds success
	 *                        of the function
	 *
	 * @return     The restricted view of the enemy
	 */
	EnemyUnitView * GetAttackTarget(int * success);
	/**
	 * Gets the velocity vector of the actor
	 *
	 * @return     The velocity vector
	 */
	physics::Vector2D GetVelocity();
	/**
	 * Gets the Actor's attack range
	 *
	 * @return     The attack range
	 */
	int64_t GetAttackRange();
	/**
	 * Gets the position vector of the actor
	 *
	 * @return     The position vector
	 */
	physics::Vector2D GetPosition();
	/**
	 * Gets the path planner helper
	 *
	 * @return     The path planner helper
	 */
	PathPlannerHelperView GetPathPlannerHelper();
};

typedef UnitView SwordsmanView;
typedef UnitView ScoutView;
typedef UnitView KingView;
typedef UnitView FlagView;
typedef UnitView BaseView;
typedef EnemyUnitView EnemySwordsmanView;
typedef EnemyUnitView EnemyMagicianView;
typedef EnemyUnitView EnemyScoutView;
typedef EnemyUnitView EnemyKingView;
typedef EnemyUnitView EnemyFlagView;
typedef EnemyUnitView EnemyBaseView;

class EnemyTowerView: public EnemyUnitView {
private:
	/**
	 * Radius around tower that units need to be present to contend for
	 * tower ownership
	 * Measured in coordinates
	 */
	int64_t contention_radius;
	/**
	 * The present contention score
	 * Varies between -max_contention_score and max_contention_score
	 */
	float contention_score;
	/**
	 * The contention score a player must reach to attain tower ownership
	 * Calculated as sum of difference in number of units over time
	 */
	int64_t max_contention_score;
public:
	explicit EnemyTowerView(Tower * tower);
	/**
	 * Gets the contention radius of the tower
	 *
	 * @return     The contention_radius
	 */
	int64_t GetContentionRadius();
	/**
	 * Gets the contention score for the tower
	 *
	 * @return     The contention_score
	 */
	float GetContentionScore();
	/**
	 * Gets the maximum contention score for the tower
	 *
	 * @return     The max_contention_score
	 */
	int64_t GetMaxContentionScore();
};

class TowerView: public UnitView {
private:
	/**
	 * Speed of a fire_ball fired from the tower
	 */
	int64_t fire_ball_speed;
	/**
	 * Time to live for the fire_balls fired
	 */
	int64_t fire_ball_ttl;
	/**
	 * Radius around tower that units need to be present to contend for
	 * tower ownership
	 * Measured in coordinates
	 */
	int64_t contention_radius;
	/**
	 * The present contention score
	 * Varies between -max_contention_score and max_contention_score
	 */
	float contention_score;
	/**
	 * The contention score a player must reach to attain tower ownership
	 * Calculated as sum of difference in number of units over time
	 */
	int64_t max_contention_score;
public:
	explicit TowerView(Tower * tower);
	/**
	 * Gets a fire_ball's lifetime
	 *
	 * @return     The time to live for the fire_ball
	 */
	int64_t GetFireBallTtl();
	/**
	 * Gets the speed of a fire_ball fired from the tower
	 *
	 * @return     The fire_ball speed
	 */
	int64_t GetFireBallSpeed();
	/**
	 * Gets the contention radius of the tower
	 *
	 * @return     The contention_radius
	 */
	int64_t GetContentionRadius();
	/**
	 * Gets the contention score for the tower
	 *
	 * @return     The contention_score
	 */
	float GetContentionScore();
	/**
	 * Gets the maximum contention score for the tower
	 *
	 * @return     The max_contention_score
	 */
	int64_t GetMaxContentionScore();
};

class MagicianView: public UnitView {
private:
	/**
	 * Speed of a fire_ball fired from the tower
	 */
	int64_t fire_ball_speed;
	/**
	 * Time to live for the fire_balls fired
	 */
	int64_t fire_ball_ttl;
public:
	explicit MagicianView(Magician * magician);
	/**
	 * Gets a fire_ball's lifetime
	 *
	 * @return     The time to live for the fire_ball
	 */
	int64_t GetFireBallTtl();
	/**
	 * Gets the speed of a fire_ball fired from the tower
	 *
	 * @return     The fire_ball speed
	 */
	int64_t GetFireBallSpeed();
};

}

#endif
