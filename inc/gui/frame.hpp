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
#include "types/component.hpp"

namespace Sorcery {

// Forward Declarations
class Display;
class System;

class Frame: public sf::Transformable, public sf::Drawable {

	public:

		// Constructors
		Frame(System *system);
		Frame(System *system, Display *display, const Component layout);

		Frame(sf::Texture *texture, const unsigned int width_units,
			const unsigned int height_units, const unsigned long long colour,
			const unsigned long long bg_colour, const unsigned int alpha);
		Frame() = delete;

		// Public Members
		sf::Sprite sprite;

		// Public Methods
		auto get_width() const -> unsigned int;
		auto get_height() const -> unsigned int;

	private:

		// Private Methods
		auto virtual draw(
			sf::RenderTarget &target, sf::RenderStates states) const -> void;

		// Private Members
		System *_system;
		Display *_display;

		std::array<sf::IntRect, 8> _frame_parts;
		std::array<sf::Sprite, 8> _frame_sprites;
		sf::RenderTexture _rtexture;
		sf::Texture *_texture;
		sf::Texture _frame_texture;
		sf::Texture _bg_texture;

		Component _layout;

		unsigned int _width_units;
		unsigned int _height_units;
		unsigned long long _colour;
		unsigned long long _bg_colour;
		unsigned int _alpha;
		sf::Sprite _frame;
		sf::RectangleShape _bg;
		unsigned int _texture_w;
		unsigned int _texture_h;
};

}
