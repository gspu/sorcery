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
#include "types/define.hpp"
#include "types/enum.hpp"
#include "types/include.hpp"
#include "types/item.hpp"

namespace Sorcery {

// Forward Declarations
class ItemType;

class Inventory {

	public:
		// Constructors
		Inventory();

		// Destructor
		~Inventory() = default;

		// Overloaded Operators
		auto operator[](const unsigned int slot) -> std::optional<Item *>;
		auto friend operator<<(std::ostream &out_stream,
							   const Inventory &inventory) -> std::ostream &;

		// Serialisation
		template <class Archive> auto serialize(Archive &archive) -> void {
			archive(_items);
		}

		// Public Methods
		auto clear() -> void;
		auto size() const -> unsigned int;
		auto is_full() const -> bool;
		auto is_empty() const -> bool;
		auto get_empty_slots() const -> unsigned int;
		auto add(Item item) -> void;
		auto add_type(const ItemType &item_type) -> bool;
		auto add_type(const ItemType &item_type, const bool known) -> bool;
		auto add_type(const ItemType &item_type, const bool usable,
					  const bool known) -> bool;
		auto unequip_all() -> void;
		auto items() const -> std::vector<Item>;
		auto has_unidentified_items() const -> bool;
		auto has_cursed_items() const -> bool;
		auto has_cursed_equipped_item_category(
			const Enums::Items::Category category) const -> bool;
		auto equip_item(const unsigned int slot) -> bool;
		auto drop_item(const unsigned int slot) -> bool;
		auto discard_item(const unsigned int slot) -> bool;
		auto is_equipped_cursed(const unsigned int slot) -> bool;
		auto identify_item(const unsigned int slot, const unsigned int roll,
						   const unsigned int id_chance,
						   const unsigned int curse_chance)
			-> Enums::Items::IdentifyOutcome;
		auto get(const unsigned int slot) -> Item;
		auto has(const unsigned int slot) const -> bool;

		auto unequip_item(const unsigned int slot) -> bool;

		auto invoke_item(const unsigned int slot) -> bool;
		auto use_item(const unsigned int slot) -> bool;

	private:
		// Private Methods
		auto
		_has_equipped_item_category(const Enums::Items::Category category) const
			-> bool;
		auto _has_cursed_equipped_item_category(
			const Enums::Items::Category category) const -> bool;
		auto _unequip_item_category(const Enums::Items::Category category)
			-> bool;

		// Private Members
		std::vector<Item> _items;
};

}
