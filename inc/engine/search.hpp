// Copyright (C) 2024 Dave Moore
//
// This file is part of Sorcery: Shadows under Llylgamyn.
//
// Sorcery: Shadows under Llylgamyn is free software: you can redistribute
// it and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 2 of the License,
// or (at your option) any later version.
//
// Sorcery: Shadows under Llylgamyn is distributed in the hope that it will
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
#include "gui/frame.hpp"
#include "types/component.hpp"

namespace Sorcery {

// Forward Declarations
class Display;
class Factory;
class Game;
class Graphics;
class System;

// Search a square
class Search: public sf::Transformable, public sf::Drawable {

	public:

		// Constructors
		Search(System *system, Display *display, Graphics *graphics, Game *game,
			Component layout);
		Search() = delete;

		// Public Members
		unsigned int width;
		unsigned int height;
		bool characters_here;

		// Public Methods
		auto refresh() -> void;

	private:

		// Private Methods
		auto virtual draw(
			sf::RenderTarget &target, sf::RenderStates states) const -> void;

		// Private Members
		System *_system;
		Display *_display;
		Graphics *_graphics;
		Game *_game;
		Component _layout;
		std::unique_ptr<Frame> _frame;
		std::vector<sf::Text> _texts;
		std::vector<sf::Sprite> _sprites;
		IconStorage _icons;
		std::unique_ptr<Factory> _factory;
};

}