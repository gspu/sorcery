
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

#include "frontend/bestiary.hpp"
#include "core/display.hpp"
#include "core/game.hpp"
#include "core/graphics.hpp"
#include "core/input.hpp"
#include "core/system.hpp"
#include "core/window.hpp"
#include "frontend/define.hpp"
#include "gui/menupaged.hpp"
#include "gui/monsterdisplay.hpp"
#include "resources/componentstore.hpp"
#include "resources/monsterstore.hpp"
#include "resources/texturestore.hpp"
#include "types/component.hpp"
#include "types/enum.hpp"
#include "types/monstertype.hpp"

// Standard Constructor
Sorcery::Bestiary::Bestiary(
	System *system, Display *display, Graphics *graphics, Game *game)
	: _system{system}, _display{display}, _graphics{graphics}, _game{game} {
}

// Standard Destructor
Sorcery::Bestiary::~Bestiary() {

	_display->stop_bg_movie();
}

auto Sorcery::Bestiary::start() -> int {

	// Clear the window
	_generate_display();
	_window->clear();

	// Play the background movie!
	_display->fit_bg_movie();
	_display->start_bg_movie();

	_display->set_input_mode(WIM::NAVIGATE_MENU);
	_selected = 0;

	if (auto result{_do_event_loop()}; result == MDR::EXIT) {

		// Shutdown
		_display->shutdown_SFML();
		return EXIT_ALL;
	}

	return EXIT_MODULE;
}

auto Sorcery::Bestiary::stop() -> void {

	_display->stop_bg_movie();
}

auto Sorcery::Bestiary::_generate_display() -> void {

	_reset_components();
	_initalise_components();
	_display->generate("bestiary");
	_place_components();
}

auto Sorcery::Bestiary::_reset_components() -> void {

	if (_menu.get())
		_menu.reset();
}

auto Sorcery::Bestiary::_place_components() -> void {

	_menu->setPosition((*_display->layout)["bestiary:menu"].pos());
	_monster_display->setPosition(
		(*_display->layout)["bestiary:monster_display"].pos());
}

auto Sorcery::Bestiary::_initalise_components() -> void {

	// Get the Window and Graphics to Display
	_window = _display->window->get_window();

	_menu = std::make_unique<MenuPaged>(
		_system, _display, _graphics, _game, MTP::BESTIARY, 10);
	_menu->set_current_page(0);

	const auto menu_c{(*_display->layout)["bestiary:menu"]};
	_menu->generate(menu_c, true);

	_monster_display =
		std::make_unique<MonsterDisplay>(_system, _display, _graphics, _game);
}

auto Sorcery::Bestiary::_refresh_display() -> void {

	if (_display->layout->refresh_if_needed())
		_generate_display();

	_display->start_bg_movie();
	_display->update_bg_movie();
	_display->draw_bg_movie();

	_draw();
	_window->display();
}

auto Sorcery::Bestiary::_draw() -> void {

	_display->display("bestiary");

	_menu->generate((*_display->layout)["bestiary:menu"]);
	_window->draw(*_menu);

	_window->draw(_known_gfx);
	_window->draw(*_monster_display);

	_display->display_overlay();
	_display->display_cursor();
}

auto Sorcery::Bestiary::_update_display() -> void {

	if (_menu->items.at(_selected.value_or(0)).type == MIT::ENTRY) {

		const auto gfx_c{(*_display->layout)["bestiary:picture"]};
		const auto idx{_menu->items.at(_selected.value_or(0)).index};

		const auto type{
			(*_game->monsterstore)[magic_enum::enum_cast<MTI>(idx).value()]};
		_known_gfx =
			_graphics->textures->get(type.get_known_gfx(), GTT::KNOWN_CREATURE)
				.value();
		_known_gfx.setPosition(gfx_c.pos());
		_known_gfx.setScale(gfx_c.scl());
		_monster_display->set(idx);
	}
};

auto Sorcery::Bestiary::_do_event_loop() -> std::optional<MDR> {

	// And do the main loop
	while (_window->isOpen()) {
		sf::Event event{};
		while (_window->pollEvent(event)) {
			auto const result = _handle_input(event);
			if (result) {
				if (result.value() == MDR::CLOSE)
					return MDR::CLOSE;
				if (result.value() == MDR::BACK)
					return MDR::BACK;
				if (result.value() == MDR::EXIT)
					return MDR::EXIT;
			}
		}

		_window->clear();
		_refresh_display();
		_window->display();
	}

	return std::nullopt;
}

auto Sorcery::Bestiary::_handle_input(const sf::Event &event)
	-> std::optional<MDR> {

	// Check for Window Close
	if (event.type == sf::Event::Closed)
		return MDR::EXIT;

	if (_system->input->check(CIN::CANCEL, event))
		return MDR::CLOSE;

	if (_system->input->check(CIN::BACK, event))
		return MDR::BACK;

	// Handle enabling help overlay
	if (_system->input->check(CIN::SHOW_CONTROLS, event)) {
		_display->show_overlay();
		return std::nullopt;
	} else
		_display->hide_overlay();

	if (_system->input->check(CIN::UP, event)) {
		_selected = _menu->choose_previous();
		const auto menu_c{(*_display->layout)["bestiary:menu"]};
		_menu->generate(menu_c, true);
		_update_display();
	} else if (_system->input->check(CIN::DOWN, event)) {
		_selected = _menu->choose_next();
		const auto menu_c{(*_display->layout)["bestiary:menu"]};
		_menu->generate(menu_c, true);
		_update_display();
	} else if (_system->input->check(CIN::LEFT, event)) {
		_selected = _menu->go_to_previous_page();
		const auto menu_c{(*_display->layout)["bestiary:menu"]};
		_menu->generate(menu_c, true);
		_update_display();
	} else if (_system->input->check(CIN::RIGHT, event)) {
		_selected = _menu->go_to_next_page();
		const auto menu_c{(*_display->layout)["bestiary:menu"]};
		_menu->generate(menu_c, true);
		_update_display();
	} else if (_system->input->check(CIN::MOVE, event)) {
		_selected = _menu->set_mouse_selected(_display->get_cur());
		if (_selected) {
			const auto menu_c{(*_display->layout)["bestiary:menu"]};
			_menu->generate(menu_c, true);
			_update_display();
		}
	} else if (_system->input->check(CIN::CONFIRM, event)) {

		// We have selected something from the menu
		if (_selected) {
			// TODO
			// const MIM opt{(*_selected.value()).item};
			// if (opt == MIM::ITEM_RETURN)
			//	return MDR::BACK;
		}
	}

	return std::nullopt;
}
