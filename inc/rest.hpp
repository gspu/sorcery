// Copyright (C) 2021 Dave Moore
//
// This file is part of Sorcery: Shadows under Llylgamyn
//
// Sorcery is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Sorcery is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with sorcery-sfml.  If not, see <http://www.gnu.org/licenses/>.
// Additional permission under GNU GPL version 3 section 7:
//
// If you modify this Program, or any covered work, by linking or combining it
// with the libraries referred to in README (or a modified version of said
// libraries), containing parts covered by the terms of said libraries, the
// licensors of this Program grant you additional permission to convey the
// resulting work.

#pragma once

#include "character.hpp"
#include "display.hpp"
#include "graphics.hpp"
#include "main.hpp"
#include "system.hpp"

namespace Sorcery {

class Rest: public sf::Transformable, public sf::Drawable {

	public:

		Rest(System *system, Display *display, Graphics *graphics, Character *character, RestMode mode, RestType type);

	private:

		// Private Methods
		auto virtual draw(sf::RenderTarget &target, sf::RenderStates states) const -> void;
};

}