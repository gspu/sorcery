// Copyright (C) 2021 Dave Moore
//
// This file is part of Sorcery: Dreams of the Mad Overlord.
//
// Sorcery: Dreams of the Mad Overlord is free software: you can redistribute
// it and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 2 of the License,
// or (at your option) any later version.
//
// Sorcery: Dreams of the Mad Overlord is distributed in the hope that it wil
// be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sorcery: Dreams of the Mad Overlord.  If not,
// see <http://www.gnu.org/licenses/>.
//
// If you modify this program, or any covered work, by linking or combining
// it with the libraries referred to in README (or a modified version of
// said libraries), containing parts covered by the terms of said libraries,
// the licensors of this program grant you additional permission to convey
// the resulting work.

#include "roster.hpp"

// Standard Constructor
Sorcery::Roster::Roster(
	System *system, Display *display, Graphics *graphics, Game *game, RosterMode mode)
	: _system{system}, _display{display}, _graphics{graphics}, _game{game}, _mode{mode} {

	// Get the Window and Graphics to Display
	_window = _display->window->get_window();
	_current_character = std::nullopt;

	// Same object can be used in three different modes
	switch (_mode) {
	case RosterMode::INSPECT:
		_screen_key = "roster_inspect";
		break;
	case RosterMode::EDIT:
		_screen_key = "roster_edit";
		break;
	case RosterMode::DELETE:
		_screen_key = "roster_delete";
		break;
	default:
		break;
	}

	_character_panel = std::make_unique<CharPanel>(_system, _display, _graphics);

	_edit = std::make_unique<Edit>(_system, _display, _graphics, _game);

	_dialog_delete = std::make_unique<Dialog>(_system, _display, _graphics,
		(*_display->layout)["roster_delete:dialog_delete_character"],
		(*_display->layout)["roster_delete:dialog_delete_character_text"],
		WindowDialogType::CONFIRM);
	_dialog_delete->setPosition((*_display->layout)["roster_delete:dialog_delete_character"].x,
		(*_display->layout)["roster_delete:dialog_delete_character"].y);
}

// Standard Destructor
Sorcery::Roster::~Roster() {}

auto Sorcery::Roster::start() -> std::optional<MenuItem> {

	_game->reload_characters();

	// Do the Menu here when it has access to the game characters
	_menu.reset();
	_menu = std::make_shared<Menu>(_system, _display, _graphics, _game, MenuType::CHARACTER_ROSTER);
	_current_character_idx = -1;

	// Get the Background Display Components and load them into Display module storage (not local)
	switch (_mode) {
	case RosterMode::INSPECT:
		_display->generate_components("roster_inspect");
		break;
	case RosterMode::EDIT:
		_display->generate_components("roster_edit");
		break;
	case RosterMode::DELETE:
		_display->generate_components("roster_delete");
		break;
	default:
		break;
	}

	// Set up the Custom Components
	const Component bg_c{(*_display->layout)[_screen_key + ":background"]};
	sf::IntRect bg_rect{};
	bg_rect.width = std::stoi(bg_c["source_w"].value());
	bg_rect.height = std::stoi(bg_c["source_h"].value());
	bg_rect.top = 0;
	bg_rect.left = std::stoi(bg_c["source_w"].value()) * std::stoi(bg_c["source_index"].value());

	_bg.setTexture(_system->resources->textures[GraphicsTexture::TOWN]);
	_bg.setTextureRect(bg_rect);
	_bg.setScale(std::stof(bg_c["scale_x"].value()), std::stof(bg_c["scale_y"].value()));
	_bg.setPosition(_display->window->get_x(_bg, bg_c.x), _display->window->get_y(_bg, bg_c.y));

	const Component menu_fc{(*_display->layout)[_screen_key + ":menu_frame"]};
	_menu_frame = std::make_unique<Frame>(_display->ui_texture, WindowFrameType::NORMAL, menu_fc.w,
		menu_fc.h, menu_fc.colour, menu_fc.background, menu_fc.alpha);
	_menu_frame->setPosition(_display->window->get_x(_menu_frame->sprite, menu_fc.x),
		_display->window->get_y(_menu_frame->sprite, menu_fc.y));

	const Component cc_fc{(*_display->layout)[_screen_key + ":character_frame"]};
	_current_character_frame = std::make_unique<Frame>(_display->ui_texture,
		WindowFrameType::NORMAL, cc_fc.w, cc_fc.h, cc_fc.colour, cc_fc.background, cc_fc.alpha);
	_current_character_frame->setPosition(
		_display->window->get_x(_current_character_frame->sprite, cc_fc.x),
		_display->window->get_y(_current_character_frame->sprite, cc_fc.y));

	const Component p_fc{(*_display->layout)["roster:preview_frame"]};
	_preview_frame = std::make_unique<Frame>(_display->ui_texture, WindowFrameType::NORMAL, p_fc.w,
		p_fc.h, p_fc.colour, p_fc.background, p_fc.alpha);
	_preview_frame->setPosition(_display->window->get_x(_preview_frame->sprite, p_fc.x),
		_display->window->get_y(_preview_frame->sprite, p_fc.y));

	// Clear the window
	_window->clear();

	// Play the background movie!
	_display->fit_background_movie();
	_display->start_background_movie();

	_display->set_input_mode(WindowInputMode::NAVIGATE_MENU);
	std::optional<std::vector<MenuEntry>::const_iterator> selected{_menu->items.begin()};

	// And do the main loop
	sf::Event event{};
	while (_window->isOpen()) {
		while (_window->pollEvent(event)) {

			// Check for Window Close
			if (event.type == sf::Event::Closed)
				_window->close();

			// Handle enabling help overlay
			if (_system->input->check_for_event(WindowInput::SHOW_CONTROLS, event)) {
				_display->show_overlay();
				continue;
			} else
				_display->hide_overlay();

			if (_display->get_input_mode() == WindowInputMode::NAVIGATE_MENU) {

				if (_system->input->check_for_event(WindowInput::CANCEL, event))
					return std::nullopt;

				if (_system->input->check_for_event(WindowInput::BACK, event))
					return std::nullopt;

				if (_system->input->check_for_event(WindowInput::UP, event))
					selected = _menu->choose_previous();
				else if (_system->input->check_for_event(WindowInput::DOWN, event))
					selected = _menu->choose_next();
				else if (_system->input->check_for_event(WindowInput::MOVE, event))
					selected = _menu->set_mouse_selected(
						static_cast<sf::Vector2f>(sf::Mouse::getPosition(*_window)));
				else if (_system->input->check_for_event(WindowInput::CONFIRM, event)) {

					// We have selected something from the menu
					if (selected) {
						const MenuItem option_chosen{(*selected.value()).item};
						if (option_chosen == MenuItem::ET_TRAIN) {
							_display->set_input_mode(WindowInputMode::NAVIGATE_MENU);
							_current_character = std::nullopt;
							return std::nullopt;
						} else {

							if (_mode == RosterMode::INSPECT) {

								const unsigned int character_chosen{(*selected.value()).index};
								_current_character = &_game->characters.at(character_chosen);
								if (_current_character) {
									_display->set_input_mode(WindowInputMode::BROWSE_CHARACTER);
									_current_character.value()->set_view(CharacterView::SUMMARY);
								}
							} else if (_mode == RosterMode::DELETE) {

								const unsigned int character_chosen{(*selected.value()).index};
								_current_character = &_game->characters.at(character_chosen);
								if (_current_character) {
									_display->set_input_mode(
										WindowInputMode::CONFIRM_DELETE_CHARACTER);
								}
							} else if (_mode == RosterMode::EDIT) {

								_edit->start();
								_edit->stop();
								_display->generate_components("character_edit");
								_display->set_input_mode(WindowInputMode::NAVIGATE_MENU);
							}
						}
					}
				}

				if (selected) {
					if ((*selected.value()).item != MenuItem::ET_TRAIN) {
						const int character_chosen{static_cast<int>((*selected.value()).index)};
						if (character_chosen != _current_character_idx) {
							auto character{&_game->characters.at(character_chosen)};
							_character_panel->set(character);
							_current_character_idx = character_chosen;
						}
					} else {
						_character_panel->valid = false;
						_current_character_idx = -1;
					}
				}
			} else if (_display->get_input_mode() == WindowInputMode::CONFIRM_DELETE_CHARACTER) {

				auto dialog_input{_dialog_delete->handle_input(event)};
				if (dialog_input) {
					if (dialog_input.value() == WindowDialogButton::CLOSE) {
						return std::nullopt;
						_display->set_input_mode(WindowInputMode::NAVIGATE_MENU);
					} else if (dialog_input.value() == WindowDialogButton::YES) {

						// Delete a character!
						_system->database->delete_character(
							_game->get_id(), _current_character_idx);

						// Need to reload the menu!
						_game->reload_characters();
						_menu->reload();

						// And select the first one in the list after one is deleted
						_menu->choose_first();
						_display->set_input_mode(WindowInputMode::NAVIGATE_MENU);
						continue;
					} else if (dialog_input.value() == WindowDialogButton::NO) {
						_display->set_input_mode(WindowInputMode::NAVIGATE_MENU);
					}
				}

			} else {

				if (_system->input->check_for_event(WindowInput::LEFT, event))
					_current_character.value()->left_view();
				else if (_system->input->check_for_event(WindowInput::RIGHT, event))
					_current_character.value()->right_view();
				else if (_system->input->check_for_event(WindowInput::CANCEL, event)) {
					_display->set_input_mode(WindowInputMode::NAVIGATE_MENU);
					_current_character = std::nullopt;
				} else if (_system->input->check_for_event(WindowInput::BACK, event)) {
					_display->set_input_mode(WindowInputMode::NAVIGATE_MENU);
					_current_character = std::nullopt;
				} else if (_system->input->check_for_event(WindowInput::CONFIRM, event)) {
					_current_character.value()->right_view();
				} else if (_system->input->check_for_event(WindowInput::UP, event)) {
					if (_current_character.value()->get_view() == CharacterView::MAGE_SPELLS)
						_current_character.value()->dec_highlighted_spell(SpellType::MAGE);
					else if (_current_character.value()->get_view() == CharacterView::PRIEST_SPELLS)
						_current_character.value()->dec_highlighted_spell(SpellType::PRIEST);

				} else if (_system->input->check_for_event(WindowInput::DOWN, event)) {
					if (_current_character.value()->get_view() == CharacterView::MAGE_SPELLS)
						_current_character.value()->inc_highlighted_spell(SpellType::MAGE);
					else if (_current_character.value()->get_view() == CharacterView::PRIEST_SPELLS)
						_current_character.value()->inc_highlighted_spell(SpellType::PRIEST);
				} else if (_system->input->check_for_event(WindowInput::MOVE, event)) {
					if (_current_character.value()->check_for_mouse_move(
							sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(*_window).x),
								static_cast<float>(sf::Mouse::getPosition(*_window).y)))) {
						_current_character.value()->set_view(
							_current_character.value()->get_view());
					}
				}
			}
		}

		_window->clear();

		// Update Background Movie
		_display->start_background_movie();
		_display->update_background_movie();
		_display->draw_background_movie();

		_draw();
		_window->display();
	}

	return std::nullopt;
}

auto Sorcery::Roster::stop() -> void {

	_display->set_input_mode(WindowInputMode::NAVIGATE_MENU);

	// Stop the background movie!
	_display->stop_background_movie();
}

auto Sorcery::Roster::_draw() -> void {

	// Display Components
	_display->display_components("roster");
	if (_display->get_input_mode() == WindowInputMode::BROWSE_CHARACTER) {
		if (_current_character) {

			// Character Preview
			_window->draw(*_preview_frame);
			if (_character_panel->valid) {
				_character_panel->setPosition((*_display->layout)["roster:info_panel"].x,
					(*_display->layout)["roster:info_panel"].y);
				_window->draw(*_character_panel);
			}

			// If we have a character
			_window->draw(*_current_character_frame);

			_current_character.value()->setPosition(
				(*_display->layout)[_screen_key + ":character"].x,
				(*_display->layout)[_screen_key + ":character"].y);
			_current_character.value()->update();
			_window->draw(*_current_character.value());
		}
	} else if (_display->get_input_mode() == WindowInputMode::CONFIRM_DELETE_CHARACTER) {

		// Menu Frame
		_window->draw(*_menu_frame);
		_window->draw(*_preview_frame);

		// And the Menu
		_menu->generate((*_display->layout)[_screen_key + ":menu"]);
		const sf::Vector2f menu_pos((*_display->layout)[_screen_key + ":menu"].x,
			(*_display->layout)[_screen_key + ":menu"].y);
		_menu->setPosition(menu_pos);
		_window->draw(*_menu);

		// Character Preview
		if (_character_panel->valid) {
			_character_panel->setPosition((*_display->layout)["roster:info_panel"].x,
				(*_display->layout)["roster:info_panel"].y);
			_window->draw(*_character_panel);
		}

		if (_current_character) {
			_dialog_delete->update();
			_window->draw(*_dialog_delete);
		}
	} else {

		// Menu Frame
		_window->draw(*_menu_frame);
		_window->draw(*_preview_frame);

		// And the Menu
		_menu->generate((*_display->layout)[_screen_key + ":menu"]);
		const sf::Vector2f menu_pos((*_display->layout)[_screen_key + ":menu"].x,
			(*_display->layout)[_screen_key + ":menu"].y);
		_menu->setPosition(menu_pos);
		_window->draw(*_menu);

		// Character Preview
		if (_character_panel->valid) {
			_character_panel->setPosition((*_display->layout)["roster:info_panel"].x,
				(*_display->layout)["roster:info_panel"].y);
			_window->draw(*_character_panel);
		}
	}

	// And finally the Cursor
	_display->display_overlay();
	_display->display_cursor();
}