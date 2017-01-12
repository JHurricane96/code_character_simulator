/**
 * @file king.h
 * Defines the King class
 */

#ifndef STATE_ACTOR_KING_H
#define STATE_ACTOR_KING_H

#include <cstdint>
#include "actor/actor.h"
#include "actor/king.fwd.h"
#include "actor/flag.fwd.h"
#include "state_export.h"

namespace state {
/**
 * Class for the king
 *
 * The king is the only unit that can captue a flag and carry it
 */
class STATE_EXPORT King: public Actor {
private:
	/**
	 * Flag that this king carries
	 *
	 * nullptr if this king isn't carrying one
	 */
	Flag * flag;
public:
	King(
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
		int64_t attack_speed
	);
	/**
	 * Assigns flag to this->flag
	 *
	 * @param      flag  The flag
	 */
	void CaptureFlag(Flag * flag);
	/**
	 * Sets king's flag to nullptr
	 */
	void DropFlag();
	/**
	 * Determines if this king has a flag
	 *
	 * @return     false if king's flag is nullptr, true otherwise
	 */
	bool HasFlag();
	/**
	 * Update function to be called every tick
	 *
	 * @param[in]  delta_time  The difference in time between the
	 *                         previous and current Update calls
	 */
	void Update(float delta_time);
	/**
	 * Merges this, a King in the main state, with the corresponding
	 * King in a player's state
	 *
	 * @param[in]  king    The player's King
	 * @param[in]  actors  The player's state's Actors. Any
	 *                     references to the other state's Actors
	 *                     are replaced by this state's Actors
	 */
	void MergeWithBuffer(
		const King * king,
		std::vector<std::shared_ptr<Actor> > actors
	);
	/**
	 * Merges this, a King in a player's state, with the corresponding
	 * King in the main state
	 *
	 * @param[in]  king    The main state's King
	 * @param[in]  actors  The player's state's Actors. Any
	 *                     references to the other state's Actors
	 *                     are replaced by this state's Actors
	 */
	void MergeWithMain(
		const King * king,
		std::vector<std::shared_ptr<Actor> > actors
	);
};

}

#endif
