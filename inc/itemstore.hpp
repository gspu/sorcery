// Copyright (C) 2023 Dave Moore
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

#include "item.hpp"
#include "itemtype.hpp"
#include "main.hpp"
#include "system.hpp"

namespace Sorcery {

class ItemStore {

	public:

		ItemStore(System *system, const std::filesystem::path filename);
		ItemStore() = delete;

		// Overload operators
		auto operator[](ItemTypeID item_type_id) const -> ItemType;
		auto operator()(ItemCategory) const -> std::optional<std::vector<ItemType>>;

		// Public methods
		auto get(ItemTypeID item_type_id) const -> Item;
		auto get(ItemTypeID min_item_type_id, ItemTypeID max_item_type_id) const -> Item;

	private:

		// Private members
		System *_system;
		std::map<ItemTypeID, ItemType> _items;
		bool _loaded;

		// Private methods
		auto _load(const std::filesystem::path filename) -> bool;
};

}