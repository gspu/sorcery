// Copyright (C) 2024 Dave Moore
//
// This file is part of Sorcery: Shadows under Llylgamyn.
//
// Sorcery: Shadows under Llylgamyn is free software: you can redistribute
// it and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 2 of the License,
// or (at your option) any later version.
//
// Sorcery: Shadows under Llylgamyn is distributed in the hope that it wil
// be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sorcery: Shadows under Llylgamyn.  If not,
// see <http://www.gnu.org/licenses/>.
//
// If you modify this program, or any covered work, by linking or combining
// it with the libraries referred to in README (or a modified version of
// said libraries), containing parts covered by the terms of said libraries,
// the licensors of this program grant you additional permission to convey
// the resulting work.

#pragma once

#include "common/include.hpp"
#include "common/type.hpp"
#include "types/inventory.hpp"

namespace Sorcery {

// Forward Declarations
class Display;
class Graphics;
class System;
class ItemStore;

class Character {

	public:

		// Constructors
		Character();
		Character(System *system, Display *display, Graphics *graphics, ItemStore *itemstore);

		// Destructor
		~Character() = default;

		// Overloaded Operators
		auto operator[](const CAB &key) -> int &;
		auto friend operator<<(std::ostream &out_stream, const Character &character) -> std::ostream &;

		// Serialisation
		template <class Archive> auto serialize(Archive &archive) -> void {
			archive(_location, coordinate, depth, _version, _name, _race, _class, _alignment, _start_attr, _cur_attr,
				_max_attr, _st_points, _portrait_index, _abilities, _priest_max_sp, _priest_cur_sp, _mage_max_sp,
				_mage_cur_sp, _status, _hidden, _spells_known, _legated, inventory, _current_stage);
		}

		// Public Methods
		auto reset(const CHS stage) -> void;
		auto get_stage() const -> CHS;
		auto set_stage(const CHS stage) -> void;
		auto set_pos_class() -> void;
		auto finalise() -> void;
		auto level_up() -> std::string;
		auto level_down() -> void;
		auto alignment_to_str(const CAL alignment) const -> std::string;
		auto race_to_str(const CHR race) const -> std::string;
		auto class_to_str(const CHC cclass) const -> std::string;
		auto create_random() -> void;
		auto create_quick() -> void;
		auto create_class_alignment(const CHC cclass, const CAL alignment) -> void;
		auto set_start_attr() -> void;
		auto get_name() const -> std::string;
		auto get_name_and_loc() const -> std::string;
		auto get_name_and_status() const -> std::string;
		auto set_name(std::string_view value) -> void;
		auto get_race() const -> CHR;
		auto set_race(const CHR &value) -> void;
		auto get_location() const -> CHL;
		auto set_location(const CHL value) -> void;
		auto get_level() const -> int;
		auto set_level(const int &value) -> void;
		auto get_class() const -> CHC;
		auto set_class(const CHC &value) -> void;
		auto get_alignment() const -> CAL;
		auto set_alignment(const CAL &value) -> void;
		auto get_cur_attr() const -> CharacterAttributes;
		auto get_pos_class() const -> CharacterClassQualified;
		auto get_start_attr() const -> CharacterAttributes;
		auto get_points_left() const -> unsigned int;
		auto set_points_left(const unsigned int &value) -> void;
		auto get_start_points() const -> unsigned int;
		auto get_num_pos_class() const -> unsigned int;
		auto set_start_points(const unsigned int &value) -> void;
		auto get_condition() const -> std::string;
		auto get_short_cond() const -> std::string;
		auto get_cur_attr(const CAR attribute) const -> unsigned int;
		auto get_start_attr(const CAR attribute) const -> unsigned int;
		auto set_cur_attr(const CAR attribute, const int adjustment) -> void;
		auto get_portrait_index() const -> unsigned int;
		auto set_portrait_index(const unsigned int value) -> void;
		auto get_spell_points(const SPT type, const SPS status) const -> std::optional<SpellPoints>;
		auto get_gold() const -> unsigned int;
		auto set_gold(const unsigned int value) -> void;
		auto grant_gold(const int value) -> void;
		auto summary_text() -> std::string;
		auto get_method() const -> CRM;
		auto set_method(const CRM value) -> void;
		auto get_summary() -> std::string;
		auto create_spells() -> void;
		auto reset_spells() -> void;
		auto replenish_spells() -> void;
		auto set_spells() -> void;
		auto get_status() const -> CHT;
		auto get_status_string() const -> std::string;
		auto get_loc_str() const -> std::string;
		auto get_summary_and_out() -> std::string;
		auto set_status(CHT value) -> void;
		auto is_poisoned() const -> bool;
		auto get_poisoned_rate() const -> int;
		auto set_poisoned_rate(int value) -> void;
		auto get_poisoned_string() const -> std::string;
		auto get_hp_summary() const -> std::string;
		auto get_short_hp_summary() const -> std::string;
		auto change_class(const CHC &value) -> void;
		auto legate(const CAL &value) -> void;
		auto is_legated() const -> bool;
		auto get_version() const -> int;
		auto get_sb_text(const int position) -> std::string;
		auto get_current_hp() const -> int;
		auto get_max_hp() const -> int;
		auto set_current_hp(const int hp) -> void;
		auto get_hp_adjustment() const -> int;
		auto set_hp_gain_per_turn(const int adjustment) -> void;
		auto set_hp_loss_per_turn(const int adjustment) -> void;
		auto reset_adjustment_per_turn() -> void;
		auto get_hp_adjustment_symbol() const -> char;
		auto get_cur_ac() const -> int;
		auto get_cur_to_hit() const -> int;
		auto get_cur_num_attacks() const -> int;
		auto get_cur_xp() const -> int;
		auto get_next_xp() const -> int;
		auto get_age() const -> int;
		auto set_age(const int adjustment) -> void;
		auto grant_xp(const int adjustment) -> int;
		auto can_level() const -> bool;
		auto get_cure_cost() const -> unsigned int;
		auto get_ress_chance(bool ashes) -> unsigned int;
		auto damage(const unsigned int adjustment) -> bool;
		auto heal(const unsigned int adjustment) -> void;
		auto attributes() -> CharacterAttributes &;
		auto abilities() -> CharacterAbilities &;
		auto spells() -> std::vector<Spell> &;
		auto priest_cur_sp() -> SpellPoints &;
		auto mage_cur_sp() -> SpellPoints &;
		auto priest_max_sp() -> SpellPoints &;
		auto mage_max_sp() -> SpellPoints &;

		// Public Members
		std::optional<Coordinate> coordinate;
		std::optional<int> depth;
		Inventory inventory;

	private:

		// Private Methods
		auto _generate_start_info() -> void;
		auto _regenerate_start_info() -> void;
		auto _legate_start_info() -> void;
		auto _generate_secondary_abil(bool initial, bool change_class, bool legate) -> void;
		auto _set_starting_sp() -> void;
		auto _reset_starting_sp() -> void;
		auto _clear_sp() -> void;
		auto _set_start_spells() -> void;
		auto _reset_start_spells() -> void;
		auto _get_hp_per_level() -> int;
		auto _update_hp_for_level() -> int;
		auto _try_learn_spell(SPT spell_type, unsigned int spell_level) -> bool;
		auto _calculate_sp(SPT spell_type, unsigned int level_mod, unsigned int level_offset) -> void;
		auto _set_sp() -> bool;
		auto _get_spells_known(SPT spell_type, unsigned int spell_level) -> unsigned int;
		auto _get_xp_for_level(unsigned int level) const -> int;
		auto _get_mage_status(bool current) -> std::string;
		auto _get_priest_status(bool current) -> std::string;
		auto _get_sp_per_level(const SPT type, int level) -> std::string;
		auto _get_condition() const -> std::string;
		auto _update_stat_for_level(CAR attribute, std::string stat) -> std::string;
		auto _learn_spell(SPI spell_id) -> void;
		auto _damage(const unsigned int adjustment)
			-> bool; // returns true is character is alive, or dead if damage was fatal
		auto _heal(const unsigned int adjustment) -> void;

		// Private Members
		int _version;

		System *_system;
		Display *_display;
		Graphics *_graphics;
		ItemStore *_itemstore;
		CharacterAbilities _abilities;
		SpellPoints _priest_max_sp;
		SpellPoints _priest_cur_sp;
		SpellPoints _mage_max_sp;
		SpellPoints _mage_cur_sp;
		std::vector<Spell> _spells;
		std::map<SPI, bool> _spells_known;
		CHS _current_stage;
		std::string _name;
		CHR _race;
		CHC _class;
		CAL _alignment;
		CharacterAttributes _start_attr;
		CharacterAttributes _cur_attr;
		CharacterAttributes _max_attr;
		unsigned int _points_left;
		unsigned int _st_points;
		CharacterClassQualified _pos_classes;
		CharacterClassList _class_list;
		unsigned int _num_pos_classes;
		unsigned int _portrait_index;
		CHT _status;
		bool _hidden;
		CRM _method;
		bool _legated;
		CHL _location;
};

}
