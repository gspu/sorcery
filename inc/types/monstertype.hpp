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

#include "common/include.hpp"
#include "common/types.hpp"
#include "types/dice.hpp"
#include "types/enum.hpp"

namespace Sorcery {

class MonsterType {

	public:
		// Default Constructor
		MonsterType() = default;

		// Overloaded Operators
		auto friend operator<<(std::ostream &out_stream,
							   const MonsterType &ItemType) -> std::ostream &;

		// Public Methods
		auto get_type_id() const -> Enums::Monsters::TypeID;
		auto get_known_name() const -> std::string;
		auto get_unknown_name() const -> std::string;
		auto get_known_name_plural() const -> std::string;
		auto get_unknown_name_plural() const -> std::string;
		auto get_group_size() const -> Dice;
		auto get_traits() const -> std::string;
		auto get_weaknesses() const -> std::string;
		auto get_level() const -> unsigned int;
		auto get_known_gfx() const -> unsigned int;
		auto get_unknown_gfx() const -> unsigned int;
		auto get_hit_dice() const -> Dice;
		auto get_category() const -> Enums::Monsters::Category;
		auto get_class() const -> Enums::Monsters::Class;
		auto get_armour_class() const -> int;
		auto get_attacks() const -> std::vector<Dice>;
		auto get_attacks_str() const -> std::string;
		auto get_number_attacks() const -> unsigned int;
		auto get_attack(const unsigned int attack) const -> Dice;
		auto get_breath_weapon() const -> Enums::Monsters::Breath;
		auto get_level_drain() const -> unsigned int;
		auto get_regeneration() const -> unsigned int;
		auto get_rewards() const -> std::tuple<unsigned int, unsigned int>;
		auto get_resistances() const -> std::array<bool, 7>;
		auto get_properties() const -> std::array<bool, 7>;
		auto get_xp() const -> unsigned int;
		auto get_partners() const -> std::tuple<unsigned int, unsigned int>;
		auto get_mage_level() const -> unsigned int;
		auto get_priest_level() const -> unsigned int;
		auto get_spell_resistance() const -> unsigned int;

		auto set_type_id(const Enums::Monsters::TypeID value) -> void;
		auto set_known_name(const std::string value) -> void;
		auto set_unknown_name(const std::string value) -> void;
		auto set_known_name_plural(const std::string value) -> void;
		auto set_unknown_name_plural(const std::string value) -> void;
		auto set_group_size(const std::string value) -> void;
		auto set_level(const unsigned int value) -> void;
		auto set_known_gfx(const unsigned int value) -> void;
		auto set_unknown_gfx(const unsigned int value) -> void;
		auto set_traits(const std::string value) -> void;
		auto set_weaknesses(const std::string value) -> void;
		auto set_hit_dice(const std::string value) -> void;
		auto set_category(const Enums::Monsters::Category value) -> void;
		auto set_class(const Enums::Monsters::Class value) -> void;
		auto set_armour_class(const int value) -> void;
		auto set_attack(Dice value) -> void;
		auto set_breath_weapon(const Enums::Monsters::Breath value) -> void;
		auto set_level_drain(const unsigned int value) -> void;
		auto set_regeneration(const unsigned int value) -> void;
		auto set_rewards(const unsigned int value_1, const unsigned int value_2)
			-> void;
		auto set_resistances(std::array<bool, 7> value) -> void;
		auto set_properties(std::array<bool, 7> value) -> void;
		auto set_xp(const unsigned int value) -> void;
		auto set_partners(const unsigned int value_1,
						  const unsigned int value_2) -> void;
		auto set_priest_level(const unsigned int value) -> void;
		auto set_mage_level(const unsigned int value) -> void;
		auto set_spell_resistance(const unsigned int value) -> void;

		auto has_resistance(Enums::Monsters::Resistance value) -> bool;
		auto has_property(Enums::Monsters::Property value) -> bool;
		auto clear_attacks() -> void;

	private:
		// Private Members
		Enums::Monsters::TypeID _type;
		std::string _known_name;
		std::string _unknown_name;
		std::string _known_name_plural;
		std::string _unknown_name_plural;
		Dice _group_size;
		unsigned int _level;
		Dice _hit_dice;
		unsigned int _known_gfx;
		unsigned int _unknown_gfx;
		Enums::Monsters::Category _category;
		Enums::Monsters::Class _class;
		int _armour_class;
		std::vector<Dice> _attacks;
		Enums::Monsters::Breath _breath_weapon;
		unsigned int _level_drain;
		unsigned int _regeneration;
		unsigned int _reward_1;
		unsigned int _reward_2;
		std::array<bool, 7> _resistances;
		std::array<bool, 7> _properties;
		unsigned int _xp;
		unsigned int _partner_type_id;
		unsigned int _partner_chance;
		unsigned int _mage_level;
		unsigned int _priest_level;
		unsigned int _spell_resistance;

		std::string _weaknesses;
		std::string _traits;

		static std::random_device _device; // Shared RNG
		static std::mt19937_64 _random;
};

}
