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

#include "modules/restart.hpp"
#include "common/enum.hpp"
#include "common/type.hpp"
#include "core/display.hpp"
#include "core/game.hpp"
#include "core/graphics.hpp"
#include "core/input.hpp"
#include "core/system.hpp"
#include "core/window.hpp"
#include "gui/frame.hpp"
#include "gui/menu.hpp"
#include "resources/componentstore.hpp"
#include "resources/factory.hpp"
#include "resources/resourcemanager.hpp"
#include "types/component.hpp"

// Standard Constructor
Sorcery::Restart::Restart(
	System *system, Display *display, Graphics *graphics, Game *game)
	: _system{system}, _display{display}, _graphics{graphics}, _game{game} {

	// Get the Window and Graphics to Display
	_window = _display->window->get_window();

	// Setup the Factory
	_factory = std::make_unique<Factory>(_system, _display, _graphics, _game);
}

// Standard Destructor
Sorcery::Restart::~Restart() {
}

auto Sorcery::Restart::start(unsigned int &character_chosen)
	-> std::optional<MIM> {

	// Get the Background Display Components and load them into Display module
	// storage (not local)
	_display->generate("restart_expedition");

	_menu =
		_factory->make_menu("restart_expedition:menu", MTP::RESTART_EXPEDITION);
	_menu_frame = _factory->make_menu_frame("restart_expedition:menu_frame");
	_update_menus(); // TODO: Why?

	// Set up the Custom Components
	const Component bg_c{(*_display->layout)["restart_expedition:background"]};
	sf::IntRect bg_rect{};
	bg_rect.width = std::stoi(bg_c["source_w"].value());
	bg_rect.height = std::stoi(bg_c["source_h"].value());
	bg_rect.top = 0;
	bg_rect.left = std::stoi(bg_c["source_w"].value()) *
				   std::stoi(bg_c["source_index"].value());

	_bg.setTexture(*_system->resources->get_texture(GTX::TOWN));
	_bg.setTextureRect(bg_rect);
	_bg.setScale(
		std::stof(bg_c["scale_x"].value()), std::stof(bg_c["scale_y"].value()));
	_bg.setPosition(_display->window->get_x(_bg, bg_c.x),
		_display->window->get_y(_bg, bg_c.y));

	// Clear the window
	_window->clear();

	_display->set_input_mode(WIM::NAVIGATE_MENU);
	MenuSelect selected{_menu->items.begin()};

	// And do the main loop
	sf::Event event{};
	while (_window->isOpen()) {
		while (_window->pollEvent(event)) {

			// Check for Window Close
			if (event.type == sf::Event::Closed)
				_window->close();

			// Handle enabling help overlay
			if (_system->input->check(CIN::SHOW_CONTROLS, event)) {
				_display->show_overlay();
				continue;
			} else
				_display->hide_overlay();

			if (_system->input->check(CIN::CANCEL, event))
				return MIM::TR_EDGE_OF_TOWN;

			if (_system->input->check(CIN::BACK, event))
				return MIM::TR_EDGE_OF_TOWN;

			if (_system->input->check(CIN::UP, event))
				selected = _menu->choose_previous();
			else if (_system->input->check(CIN::DOWN, event))
				selected = _menu->choose_next();
			else if (_system->input->check(CIN::MOVE, event))
				selected = _menu->set_mouse_selected(_display->get_cur());
			else if (_system->input->check(CIN::CONFIRM, event)) {

				if (selected) {
					if (const MIM opt{(*selected.value()).item};
						opt == MIM::TR_EDGE_OF_TOWN) {
						return MIM::TR_EDGE_OF_TOWN;
					} else {
						character_chosen = (*selected.value()).index;
						return MIM::RS_RESTART;
					}
				}
			}
		}

		_window->clear();

		_draw();
		_window->display();
	}

	return std::nullopt;
}

auto Sorcery::Restart::_update_menus() -> void {

	if (_menu.get()) {
		_menu.release();
		_menu.reset();
	}

	_menu =
		_factory->make_menu("restart_expedition:menu", MTP::RESTART_EXPEDITION);
}

auto Sorcery::Restart::stop() -> void {

	_display->set_input_mode(WIM::NAVIGATE_MENU);
}

auto Sorcery::Restart::_draw() -> void {

	// Play the background movie!
	_graphics->tile_bg(_window);

	// Display Components
	_display->display("restart_expedition");

	// Menu Frame
	_window->draw(*_menu_frame);

	// And the Menu
	_menu->generate((*_display->layout)["restart_expedition:menu"]);
	_window->draw(*_menu);

	// And finally the Cursor
	_display->display_overlay();
	_display->display_cursor();
}
