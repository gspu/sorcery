// Copyright (C) 2025 Dave Moore
//
// This file is part of Sorcery.
//
// Sorcery is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 2 of the License, or (at your option) any later
// version.
//
// Sorcery is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// Sorcery.  If not, see <http://www.gnu.org/licenses/>.
//
// If you modify this program, or any covered work, by linking or combining
// it with the libraries referred to in README (or a modified version of
// said libraries), containing parts covered by the terms of said libraries,
// the licensors of this program grant you additional permission to convey
// the resulting work.

#pragma once

#include "common/define.hpp"
#include "common/include.hpp"
#include "common/types.hpp"
#include "core/include.hpp"
#include "types/enum.hpp"
#include "types/explore.hpp"
#include "types/level.hpp"

namespace Sorcery {

// Forward Declarations
class ItemStore;
class System;

// State holds the changable game data
class State {

	public:
		// Constructor
		State();
		State(System *system);

		// Serialisation
		template <class Archive> auto serialize(Archive &archive) -> void {
			archive(_version, _party, level, explored, _player_depth,
					_previous_depth, _player_pos, _previous_pos,
					_playing_facing, _lit, _turns, _log, _shop);
		}

		// Public Members
		bool valid;
		std::unique_ptr<Level> level; // current level
		std::map<int, Explore> explored;
		std::array<bool, 8> quest_item_flags;

		// Public Methods
		auto reset_shop(ItemStore *itemstore) -> void;
		auto add_character_to_party(unsigned int char_id) -> bool;
		auto check_character_in_party(unsigned int char_id) -> bool;
		auto clear_party() -> void;
		auto get_char_slot(unsigned int char_id) -> std::optional<unsigned int>;
		auto get_next_party_character(unsigned int character_id)
			-> std::optional<unsigned int>;
		auto get_previous_party_character(unsigned int character_id)
			-> std::optional<unsigned int>;
		auto get_party_characters() const -> std::vector<unsigned int>;
		auto get_party_size() const -> unsigned int;
		auto get_player_facing() const -> Enums::Map::Direction;
		auto get_player_pos() const -> Coordinate;
		auto get_player_prev_pos() const -> Coordinate;
		auto get_player_prev_depth() const -> int;
		auto party_has_members() const -> bool;
		auto get_party_char(unsigned int index) -> std::optional<unsigned int>;
		auto add_character_by_id(unsigned int char_id) -> bool;
		auto remove_character_by_id(unsigned int char_id) -> bool;
		auto remove_character_by_position(unsigned int index) -> bool;
		auto reorder_party(std::vector<unsigned int> &new_order) -> void;
		auto set(System *system) -> void;
		auto set_party(std::vector<unsigned int> candidate_party) -> void;
		auto set_current_level(Level *other) -> void;
		auto set_player_facing(const Enums::Map::Direction direction) -> void;
		auto set_player_pos(const Coordinate position) -> void;
		auto restart_expedition() -> void;
		auto set_depth(int depth) -> void;
		auto set_player_prev_depth(int depth) -> void;
		auto get_depth() const -> int;
		auto set_lit(bool lit) -> void;
		auto get_lit() const -> bool;
		auto get_turns() const -> unsigned int;
		auto pass_turn(unsigned int turns = 1) -> void;
		auto add_log_message(std::string text,
							 Enums::Internal::MessageType type) -> void;
		auto clear_log_messages() -> void;
		auto add_log_dice_roll(const std::string &message, const int dice = -1,
							   const int roll = -1, const int needed = -1)
			-> void;
		auto get_log_messages(unsigned int last = 0) const
			-> std::vector<ConsoleMessage>;
		auto print() -> void;
		auto check_shop_stock(const Enums::Items::TypeID item_type) const
			-> int;
		auto check_shop_will_sell(const Enums::Items::TypeID item_type) const
			-> bool;
		auto check_shop_will_buy(const Enums::Items::TypeID item_type) const
			-> bool;
		auto sell_to_shop(ItemStore *itemstore,
						  const Enums::Items::TypeID item_type)
			-> int; // + Gold
		auto buy_from_shop(ItemStore *itemstore,
						   const Enums::Items::TypeID item_type)
			-> int; // - Gold
		auto get_shop_display(ItemStore *itemstore,
							  const Enums::Items::TypeID item_type)
			-> std::string;

	private:
		// Private Methods
		auto _clear() -> void;
		auto _clear_explored() -> void;
		auto _restart_expedition() -> void;

		// Private Members
		System *_system;
		std::vector<unsigned int> _party;
		Coordinate _player_pos;
		Coordinate _previous_pos;
		int _previous_depth;
		Enums::Map::Direction _playing_facing;
		int _player_depth;
		bool _lit;
		int _version;
		unsigned int _turns;
		std::vector<ConsoleMessage> _log;
		std::array<ShopStock, 101> _shop; // Max ItemID + 1 as its 0-indexed
};
}
