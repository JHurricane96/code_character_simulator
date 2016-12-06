#include <iostream>
#include <string>
#include "state.pb.h"

int PopulateTerrain(IPC::State* StateMessage, std::shared_ptr<state::State>* StateVar) {

	/**
	 * Set position of terrain element
	 *
	 * @param[in]  position  The position vector
	 */
	StateMessage->set_allocated_position(StateVar->GetPosition());
	/**
	 * Set size of terrain element
	 *
	 * @param[in]  size  The side length
	 */
	StateMessage->set_size(StateVar->GetSize());
	/**
	 * Set size of terrain element
	 *
	 * @param[in]  terrain_type  The terrain typ
	 */
	StateMessage->set_terrain_type(StateVar->GetTerrainType());
	/**
	 * Set the line of sight for a given player
	 *
	 *  @param[in]  player_id  The id of the player
	 */
	StateMessage->set_los_type_player_1(StateVar->GetLos(player_id));
	StateMessage->set_los_type_player_2(StateVar->GetLos(player_id));
	/**
	 * Set the the timestamps noting when the element was last visited by 
	 * the players
	 *
	 *  @param[in]  player_id  The id of the player
	 */
	StateMessage->set_last_seen_player_1(StateVar->GetLastSeen(player_id));
	StateMessage->set_last_seen_player_2(StateVar->GetLastSeen(player_id));

	return 0;
}

int PopulateState(std::shared_ptr<state::State> StateVar) {

	/**
	 * Verify that the version of the library that we linked against is 
	 * compatible with the version of the headers we compiled against
	 */
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	IPC::State StateMessage;

	if (PopulateTerrain(&state, &StateVar) < 0) {
		std::cerr << "Failed to load terrain" <<endl;
		return -1;
	}
	if (PopulateActors(&state) < 0) {
		std::cerr << "Failed to load actors" <<endl;
		return -1;
	}
	return 0;
}
