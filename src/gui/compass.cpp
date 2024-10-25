// Copyright (C) 2024 Dave Moore
//
// This file is part of Sorcery: Shadows under Llylgamyn.
//
// Sorcery: Shadows under Llylgamyn is free software: you can redistribute it
// and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 2 of the License,
// or (at your option) any later version.
//
// Sorcery: Shadows under Llylgamyn is distributed in the hope that it will
// be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sorcery: Shadows under Llylgamyn.  If not, see
// <http://www.gnu.org/licenses/>.
//
// If you modify this Program, or any covered work, by linking or combining it
// with the libraries referred to in README (or a modified version of said
// libraries), containing parts covered by the terms of said libraries, the
// licensors of this Program grant you additional permission to convey the
// resulting work.

#include "gui/compass.hpp"
#include "core/display.hpp"
#include "core/game.hpp"
#include "core/graphics.hpp"
#include "core/state.hpp"
#include "core/system.hpp"
#include "resources/factory.hpp"
#include "resources/iconstore.hpp"

Sorcery::Compass::Compass(System *system, Display *display, Graphics *graphics,
	Game *game, Component layout)
	: _system{system}, _display{display}, _graphics{graphics}, _game{game},
	  _layout{layout} {

	_sprites.clear();
	_texts.clear();

	// Setup the Factory
	_factory = std::make_unique<Factory>(_system, _display, _graphics, _game);

	// Make the Frame
	_frame = _factory->make_comp_frame(_layout, _sprites);
}

auto Sorcery::Compass::refresh() -> void {

	_sprites.resize(1);

	auto compass{(*_graphics->icons)["direction"].value()};
	compass.setOrigin(compass.getLocalBounds().width / 2,
		compass.getLocalBounds().height / 2);
	switch (_game->state->get_player_facing()) {
	case MAD::NORTH:
		compass.setRotation(180.0f);
		break;
	case MAD::SOUTH:
		compass.setRotation(0.0f);
		break;
	case MAD::EAST:
		compass.setRotation(270.0f);
		break;
	case MAD::WEST:
		compass.setRotation(90.0f);
		break;
	default:
		break;
	}
	compass.setPosition((compass.getGlobalBounds().width / 2) +
							std::stoi(_layout["rotation_offset_x"].value()),
		(compass.getGlobalBounds().height / 2) +
			std::stoi(_layout["rotation_offset_y"].value()));
	compass.setScale(std::stof(_layout["direction_scaling"].value()),
		std::stof(_layout["direction_scaling"].value()));
	_sprites.emplace_back(compass);
}

auto Sorcery::Compass::draw(
	sf::RenderTarget &target, sf::RenderStates states) const -> void {

	states.transform *= getTransform();

	// Draw the standard components
	for (const auto &sprite : _sprites)
		target.draw(sprite, states);

	for (const auto &text : _texts)
		target.draw(text, states);
}
