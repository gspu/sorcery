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

#include "frontend/mainmenu.hpp"
#include "common/macro.hpp"
#include "common/type.hpp"
#include "core/animation.hpp"
#include "core/display.hpp"
#include "core/game.hpp"
#include "core/graphics.hpp"
#include "core/input.hpp"
#include "core/system.hpp"
#include "core/window.hpp"
#include "frontend/attractmode.hpp"
#include "gui/dialog.hpp"
#include "gui/menu.hpp"
#include "gui/text.hpp"
#include "resources/componentstore.hpp"
#include "resources/factory.hpp"

// Standard Constructor
Sorcery::MainMenu::MainMenu(
	System *system, Display *display, Graphics *graphics, Game *game)
	: _system{system}, _display{display}, _graphics{graphics}, _game{game} {

	// Get the Window and Graphics to Display
	_window = _display->window->get_window();

	// Setup the Factory
	_factory = std::make_unique<Factory>(_system, _display, _graphics, _game);

	// Create the Main Menu
	_menu_stage = MMT::ATTRACT_MODE;
	_main_menu = _factory->make_menu("main_menu_attract:main_menu", MTP::MAIN);

	// Setup Custom Components
	Component any_key_c{(*_display->layout)["main_menu_attract:press_any_key"]};
	_press_any_key = std::make_unique<Text>(_system, _display, any_key_c,
		unenum(CPE::COLOUR) | unenum(CPE::FONT) | unenum(CPE::STRING) |
			unenum(CPE::SIZE) | unenum(CPE::JUSTIFICATION));
	auto x{any_key_c.x == -1 ? _display->window->centre.x : any_key_c.x};
	auto y{any_key_c.y == -1 ? _display->window->centre.y : any_key_c.y};
	_press_any_key->setPosition(x, y);

	// Now set up attract mode data
	_attract_mode = std::make_unique<AttractMode>(
		_graphics, (*_display->layout)["main_menu_attract:attract_creatures"]);
	_attract_mode->data.clear();

	// Create the Confirmation Dialogs
	_dialog_exit =
		_factory->make_dialog("main_menu_attract:dialog_exit", WDT::CONFIRM);
	_dialog_new_game = _factory->make_dialog(
		"main_menu_attract:dialog_new_game", WDT::CONFIRM);

	_error = std::nullopt;
}

// Standard Destructor
Sorcery::MainMenu::~MainMenu() {

	_graphics->animation->stop_attract_th();
	_display->stop_bg_movie();
}

auto Sorcery::MainMenu::start(MMT menu_stage) -> std::optional<MIM> {

	// Get the Background Display Components and load them into Display module
	// storage (not local)
	_display->generate("main_menu_attract");

	// Clear the window
	_window->clear();

	// Now set up attract mode data
	_menu_stage = menu_stage;
	_attract_mode->data_temp.clear();

	// Start relevant animation worker threads
	_graphics->animation->refresh_attract();
	_graphics->animation->start_attract_th();

	// Play the background movie!
	_display->fit_bg_movie();
	_display->start_bg_movie();

	MenuSelect selected{_main_menu->items.begin()};
	if (_menu_stage == MMT::ATTRACT_MODE)
		_display->set_input_mode(WIM::MENU_ATTRACT_MODE);
	else
		_display->set_input_mode(WIM::NAVIGATE_MENU);

	// See if we have a Game to Continue;
	_set();

	// And do the main loop
	sf::Event event{};
	while (_window->isOpen()) {

		try {

			_attract_mode->data_temp = _graphics->animation->get_attract_data();
			while (_window->pollEvent(event)) {
				if (_error)
					_display->window->get_gui()->handleEvent(event);

				else {

					// If we are in normal input mode
					if (_display->get_input_mode() == WIM::MENU_ATTRACT_MODE) {

						// Check for Window Close
						if (event.type == sf::Event::Closed)
							return MIM::ITEM_ABORT;

						// Check for any key being pressed to move onto the main
						// menu
						if (_menu_stage == MMT::ATTRACT_MODE) {

							// Check for Window Close
							if (event.type == sf::Event::Closed)
								return std::nullopt;

							// Check for any key being pressed to move onto the
							// main menu
							if (_system->input->check(CIN::ANYTHING, event)) {

								_menu_stage = MMT::ATTRACT_MENU;
								_display->set_input_mode(WIM::NAVIGATE_MENU);
							}
						}
					} else if (_display->get_input_mode() ==
							   WIM::NAVIGATE_MENU) {

						// Check for Window Close
						if (event.type == sf::Event::Closed)
							return std::nullopt;

						// Handle enabling help overlay
						if (_system->input->check(CIN::SHOW_CONTROLS, event)) {
							_display->show_overlay();
							continue;
						} else
							_display->hide_overlay();

						// And handle input on the main menu
						if (_system->input->check(CIN::UP, event))
							selected = _main_menu->choose_previous();
						else if (_system->input->check(CIN::DOWN, event))
							selected = _main_menu->choose_next();
						else if (_system->input->check(CIN::MOVE, event))
							selected = _main_menu->set_mouse_selected(
								_display->get_cur());
						else if (_system->input->check(CIN::CONFIRM, event)) {
							if (selected) {

								// We have selected something from the menu
								if (const MIM opt{(*selected.value()).item};
									opt == MIM::MM_NEW_GAME) {
									_display->set_input_mode(
										WIM::CONFIRM_NEW_GAME);
								} else if (opt == MIM::MM_CONTINUE_GAME) {
									_display->set_input_mode(
										WIM::NAVIGATE_MENU);
									return MIM::MM_CONTINUE_GAME;
								} else if (opt == MIM::MM_LICENSE) {
									_display->set_input_mode(
										WIM::DISPLAY_TEXT_FILE);
									return MIM::MM_LICENSE;
								} else if (opt == MIM::MM_COMPENDIUM) {
									_display->set_input_mode(WIM::COMPENDIUM);
									return MIM::MM_COMPENDIUM;
								} else if (opt == MIM::MM_OPTIONS) {
									_display->set_input_mode(WIM::GAME_OPTIONS);
									return MIM::MM_OPTIONS;
								} else if (opt == MIM::ITEM_QUIT) {
									_display->set_input_mode(
										WIM::CONFIRM_QUIT_GAME);
								}
							}
						} else if (_system->input->check(CIN::CANCEL, event) ||
								   _system->input->check(CIN::BACK, event)) {
							_display->set_input_mode(WIM::CONFIRM_QUIT_GAME);
						}

					} else if (_display->get_input_mode() ==
							   WIM::CONFIRM_NEW_GAME) {
						if (const auto input{
								_dialog_new_game->handle_input(event)};
							input) {
							if (input.value() == WDB::CLOSE) {
								_display->set_input_mode(WIM::NAVIGATE_MENU);
								return std::nullopt;
							} else if (input.value() == WDB::YES) {
								_display->set_input_mode(WIM::NAVIGATE_MENU);
								return MIM::MM_NEW_GAME;
							} else if (input.value() == WDB::NO) {
								_display->set_input_mode(WIM::NAVIGATE_MENU);
							}
						}
					} else if (_display->get_input_mode() ==
							   WIM::CONFIRM_QUIT_GAME) {
						if (const auto input{_dialog_exit->handle_input(event)};
							input) {
							if (input.value() == WDB::CLOSE) {
								_display->set_input_mode(WIM::NAVIGATE_MENU);
								return std::nullopt;
							} else if (input.value() == WDB::YES) {
								_display->set_input_mode(WIM::NAVIGATE_MENU);
								return MIM::ITEM_QUIT;
							} else if (input.value() == WDB::NO) {
								_display->set_input_mode(WIM::NAVIGATE_MENU);
							}
						}
					}
				}
			}

		} catch (std::exception &e) {
			_error = std::make_unique<Error>(
				_display->window->get_gui(), SYE::UNHANDLED_EXCEPTION, e);
		}

		_window->clear();
		_display->start_bg_movie();
		_display->update_bg_movie();
		_display->draw_bg_movie();

		_draw();
		_display->display_overlay();
		_display->window->get_gui()->draw();
		_display->display_cursor();
		_window->display();
	}

	return std::nullopt;
}

auto Sorcery::MainMenu::stop() -> void {

	// Stop the background movie!
	_display->stop_bg_movie();
}

auto Sorcery::MainMenu::_set() -> void {

	(*_main_menu)[1].enabled = _game->valid;
	if ((*_main_menu)[1].enabled) {
		auto it{_main_menu->choose(1)};
		_main_menu->selected = it.value();
	}
}

auto Sorcery::MainMenu::_draw() -> void {

	// Only draw the attract mode if we have something to draw (to avoid timing
	// issues
	if (_attract_mode->data_temp.size() > 0) {

		const auto lerp{_graphics->animation->lerp};
		_display->display("main_menu_attract", _menu_stage);

		// Generate and draw the Attract Mode Graphics
		Component attract_c{
			(*_display->layout)["main_menu_attract:attract_creatures"]};
		_attract_mode->generate();
		_attract_mode->setScale(attract_c.scl());
		_attract_mode->set_alpha(_graphics->animation->attract_alpha);

		// Horrible - but needed since the size of the Attract Mode Graphics are
		// variable
		const sf::Vector2f attract_size{
			_attract_mode->sprite.getGlobalBounds().width *
				_attract_mode->getScale().x,
			_attract_mode->sprite.getGlobalBounds().height *
				_attract_mode->getScale().y};
		const sf::Vector2f creature_pos(
			_display->window->centre.x - (attract_size.x / 2),
			_display->window->get_y(_attract_mode->sprite, attract_c.y));
		_attract_mode->setPosition(creature_pos);
		_window->draw(*_attract_mode);

		// And either the blurb or the main menu
		if (_menu_stage == MMT::ATTRACT_MODE) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
			sf::Color adjusted{_graphics->adjust_brightness(
				sf::Color((*_display->layout)["main_menu_attract:press_any_key"]
						.colour),
				lerp)};
#pragma GCC diagnostic pop
			_press_any_key->set_fill_colour(adjusted);
			_window->draw(*_press_any_key);
		} else {

			// Draw the menu
			_main_menu->generate(
				(*_display->layout)["main_menu_attract:main_menu"]);
			_window->draw(*_main_menu);
			if (_display->get_input_mode() == WIM::CONFIRM_QUIT_GAME) {
				_dialog_exit->update();
				_window->draw(*_dialog_exit);
			} else if (_display->get_input_mode() == WIM::CONFIRM_NEW_GAME) {
				_dialog_new_game->update();
				_window->draw(*_dialog_new_game);
			}
		}
	}
}
