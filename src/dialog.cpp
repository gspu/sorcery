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

#include "dialog.hpp"

// Standard Constructor
Sorcery::Dialog::Dialog(System *system, Display *display, Graphics *graphics, Component &frame_c,
	Component &string_c, WindowDialogType type)
	: _system{system}, _display{display}, _graphics{graphics}, _frame_c{frame_c},
	  _string_c{string_c}, _type{type} {

	_strings.clear();
	_texts.clear();
	_sprites.clear();
	_rects.clear();
	_buttons.clear();
	_buttons_hl.clear();
	_highlights.clear();
	_buttons_fr.clear();

	// Get the Window
	_window = _display->window->get_window();

	// Get the Layout
	_text_c = Component((*_display->layout)["dialog:text"]);
	_buttons_c = Component((*_display->layout)["dialog:buttons"]);

	// Get the Text
	unsigned int text_width{_frame_c.w - 4};
	std::string string{(*_display->string)[string_c.string_key]};
	std::string wrapped_text = WORDWRAP(string, text_width);

	// Split the Text into lines
	const std::regex regex(R"([@]+)");
	std::sregex_token_iterator it{wrapped_text.begin(), wrapped_text.end(), regex, -1};
	std::vector<std::string> split{it, {}};
	split.erase(std::remove_if(split.begin(), split.end(),
					[](std::string const &s) {
						return s.size() == 0;
					}),
		split.end());
	_strings = split;

	// Now work out the vertical size of the Frame
	unsigned int frame_h{static_cast<unsigned int>(_strings.size())};
	switch (_type) {
	case WindowDialogType::OK:
		frame_h += 5;
		_selected = WindowDialogButton::OK;
		break;
	case WindowDialogType::CONFIRM:
		frame_h += 5;
		_selected = WindowDialogButton::NO;
		break;
	case WindowDialogType::MENU:
		// frame_h += menu_size;
		break;
	case WindowDialogType::TIMED:
		_selected = WindowDialogButton::NONE;
		frame_h += 4;
		break;
	default:
		break;
	}

	// Add the standard components - first the frame
	_frame = std::make_unique<Frame>(_display->ui_texture, WindowFrameType::NORMAL, _frame_c.w,
		frame_h, _frame_c.colour, _frame_c.background, _frame_c.alpha);
	_frame->setPosition(0, 0);

	// Then the strings
	unsigned int x{_display->window->get_cell_width() * 1};
	unsigned int y{_display->window->get_cell_height() * 1};
	int index{0};
	for (const auto &each_string : _strings) {
		sf::Text text;
		text.setFont(_system->resources->fonts[_string_c.font]);
		text.setCharacterSize(_string_c.size);
		text.setFillColor(sf::Color(_string_c.colour));
		text.setString(each_string);

		if (_string_c.justification == Justification::CENTRE) {
			text.setPosition((x + (_frame_c.w * _display->window->get_cell_width())) / 2,
				y + (index * _display->window->get_cell_height()));
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
		} else if (_string_c.justification == Justification::RIGHT) {
			text.setPosition(x, y + (index * _display->window->get_cell_height()));
			const sf::FloatRect bounds{text.getLocalBounds()};
			text.setPosition(_string_c.x - bounds.width, _string_c.y);
		} else {
			text.setPosition(x, y + (index * _display->window->get_cell_height()));
			text.setOrigin(0, text.getLocalBounds().height / 2.0f);
		}

		++index;
		_texts.push_back(text);
	}

	// Add the additional option texts/menu
	y = _display->window->get_cell_height() * (1 + static_cast<unsigned int>(_strings.size()) + 1);
	const unsigned int centre_x{(_frame_c.w * _display->window->get_cell_width()) / 2};

	switch (_type) {
	case WindowDialogType::OK: {
		const unsigned int ok_x{(centre_x - (_display->window->get_cell_width() * 2))};
		sf::Text ok_text;
		ok_text.setFont(_system->resources->fonts[_buttons_c.font]);
		ok_text.setCharacterSize(_buttons_c.size);
		ok_text.setFillColor(sf::Color(_buttons_c.colour));
		ok_text.setString((*_display->string)["DIALOG_OK"]);
		ok_text.setPosition(ok_x, y);
		_buttons[WindowDialogButton::OK] = ok_text;

		sf::Text ok_text_hl{ok_text};
		ok_text_hl.setOutlineColor(sf::Color(0, 0, 0));
		ok_text_hl.setOutlineThickness(2);
		_buttons_hl[WindowDialogButton::OK] = ok_text_hl;

		const sf::FloatRect ok_text_rect{ok_text_hl.getGlobalBounds()};
		_buttons_fr[WindowDialogButton::OK] = ok_text_rect;

		sf::RectangleShape ok_text_bg(sf::Vector2(ok_text_rect.width + 6, ok_text_rect.height + 8));
		ok_text_bg.setOrigin(0, 0 - ok_text_hl.getLocalBounds().height + 16);
		_highlights[WindowDialogButton::OK] = ok_text_bg;

	} break;
	case WindowDialogType::CONFIRM: {
		const unsigned int yes_x{(centre_x - (_display->window->get_cell_width() * 4))};
		sf::Text yes_text;
		yes_text.setFont(_system->resources->fonts[_buttons_c.font]);
		yes_text.setCharacterSize(_buttons_c.size);
		yes_text.setFillColor(sf::Color(_buttons_c.colour));
		yes_text.setString((*_display->string)["DIALOG_YES"]);
		yes_text.setPosition(yes_x, y);
		_buttons[WindowDialogButton::YES] = yes_text;

		sf::Text yes_text_hl{yes_text};
		yes_text_hl.setPosition(yes_x, y);
		yes_text_hl.setOutlineColor(sf::Color(0, 0, 0));
		yes_text_hl.setOutlineThickness(2);
		_buttons_hl[WindowDialogButton::YES] = yes_text_hl;

		const sf::FloatRect yes_text_rect{yes_text_hl.getGlobalBounds()};
		_buttons_fr[WindowDialogButton::YES] = yes_text_rect;

		sf::RectangleShape yes_text_bg(
			sf::Vector2(yes_text_rect.width + 6, yes_text_rect.height + 8));
		yes_text_bg.setPosition(yes_x, y);
		yes_text_bg.setOrigin(0, 0 - yes_text_hl.getLocalBounds().height + 16);

		_highlights[WindowDialogButton::YES] = yes_text_bg;

		const unsigned int no_x{centre_x + (_display->window->get_cell_width() * 2)};
		sf::Text no_text;
		no_text.setFont(_system->resources->fonts[_buttons_c.font]);
		no_text.setCharacterSize(_buttons_c.size);
		no_text.setFillColor(sf::Color(_buttons_c.colour));
		no_text.setString((*_display->string)["DIALOG_NO"]);
		no_text.setPosition(no_x, y);
		_buttons[WindowDialogButton::NO] = no_text;

		sf::Text no_text_hl{no_text};
		no_text_hl.setPosition(no_x, y);
		no_text_hl.setOutlineColor(sf::Color(0, 0, 0));
		no_text_hl.setOutlineThickness(2);
		_buttons_hl[WindowDialogButton::NO] = no_text_hl;

		const sf::FloatRect no_text_rect{no_text_hl.getGlobalBounds()};
		_buttons_fr[WindowDialogButton::NO] = no_text_rect;

		sf::RectangleShape no_text_bg(sf::Vector2(no_text_rect.width + 6, no_text_rect.height + 8));
		no_text_bg.setPosition(no_x, y);
		no_text_bg.setOrigin(0, 0 - no_text_hl.getLocalBounds().height + 16);
		_highlights[WindowDialogButton::NO] = no_text_bg;

	} break;
	case WindowDialogType::MENU:
		break;
	case WindowDialogType::TIMED:
		break;
	default:
		break;
	}
}

auto Sorcery::Dialog::handle_input(sf::Event event) -> std::optional<WindowDialogButton> {
	switch (_type) {

	case WindowDialogType::OK:
		// Can't be toggled
		break;
	case WindowDialogType::CONFIRM:
		if (_system->input->check_for_event(WindowInput::LEFT, event))
			toggle_highlighted();
		else if (_system->input->check_for_event(WindowInput::RIGHT, event))
			toggle_highlighted();
		else if (_system->input->check_for_event(WindowInput::YES, event))
			set_selected(WindowDialogButton::YES);
		else if (_system->input->check_for_event(WindowInput::NO, event))
			set_selected(WindowDialogButton::NO);
		else if (_system->input->check_for_event(WindowInput::CANCEL, event))
			return WindowDialogButton::NO;
		else if (_system->input->check_for_event(WindowInput::BACK, event))
			return WindowDialogButton::NO;
		else if (_system->input->check_for_event(WindowInput::MOVE, event))
			check_for_mouse_move(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*_window)));
		else if (_system->input->check_for_event(WindowInput::CONFIRM, event)) {
			std::optional<WindowDialogButton> button_chosen{check_if_option_selected(
				static_cast<sf::Vector2f>(sf::Mouse::getPosition(*_window)))};

			// Mouse click only
			if (button_chosen) {
				if (button_chosen.value() == WindowDialogButton::YES)
					return WindowDialogButton::YES;
				else if (button_chosen.value() == WindowDialogButton::NO)
					return WindowDialogButton::NO;

			} else {

				// Button/Keyboard
				if (get_selected() == WindowDialogButton::YES) {
					return WindowDialogButton::YES;
				} else if (get_selected() == WindowDialogButton::NO)
					return WindowDialogButton::NO;
			}
		}
		break;
	case WindowDialogType::MENU:
		break;
	case WindowDialogType::TIMED:
		break;
	default:
		break;
	}

	return std::nullopt;
}

auto Sorcery::Dialog::toggle_highlighted() -> WindowDialogButton {

	switch (_type) {
	case WindowDialogType::OK:
		// Can't be toggled
		break;
	case WindowDialogType::CONFIRM:
		if (_selected == WindowDialogButton::YES)
			_selected = WindowDialogButton::NO;
		else if (_selected == WindowDialogButton::NO)
			_selected = WindowDialogButton::YES;
		return _selected;
	case WindowDialogType::MENU:
		break;
	case WindowDialogType::TIMED:
		break;
	default:
		break;
	}

	return WindowDialogButton::NONE; // TODO optional
}

auto Sorcery::Dialog::check_for_mouse_move(const sf::Vector2f mouse_pos)
	-> std::optional<WindowDialogButton> {

	const sf::Vector2f global_pos{this->getPosition()};
	const sf::Vector2f local_mouse_pos{mouse_pos - global_pos};

	switch (_type) {
	case WindowDialogType::OK:
		if (_buttons_fr.at(WindowDialogButton::OK).contains(local_mouse_pos)) {
			_selected = WindowDialogButton::OK;
			return WindowDialogButton::OK;
		}
		break;
	case WindowDialogType::CONFIRM:
		if (_buttons_fr.at(WindowDialogButton::YES).contains(local_mouse_pos)) {
			_selected = WindowDialogButton::YES;
			return WindowDialogButton::YES;
		} else if (_buttons_fr.at(WindowDialogButton::NO).contains(local_mouse_pos)) {
			_selected = WindowDialogButton::NO;
			return WindowDialogButton::NO;
		}
		break;
	case WindowDialogType::MENU:
		break;
	case WindowDialogType::TIMED:
		break;
	default:
		break;
	}

	return std::nullopt;
}

// Only works for the Mouse
auto Sorcery::Dialog::check_if_option_selected(const sf::Vector2f mouse_pos)
	-> std::optional<WindowDialogButton> {

	const sf::Vector2f global_pos{this->getPosition()};
	const sf::Vector2f local_mouse_pos{mouse_pos - global_pos};

	switch (_type) {
	case WindowDialogType::OK:
		if (_buttons_fr.at(WindowDialogButton::OK).contains(local_mouse_pos)) {
			_selected = WindowDialogButton::OK;
			return WindowDialogButton::OK;
		}
	case WindowDialogType::CONFIRM:
		if (_buttons_fr.at(WindowDialogButton::YES).contains(local_mouse_pos)) {
			_selected = WindowDialogButton::YES;
			return WindowDialogButton::YES;
		} else if (_buttons_fr.at(WindowDialogButton::NO).contains(local_mouse_pos)) {
			_selected = WindowDialogButton::NO;
			return WindowDialogButton::NO;
		}
		break;
	case WindowDialogType::MENU:
		break;
	case WindowDialogType::TIMED:
		break;
	default:
		break;
	}

	return std::nullopt;
}

auto Sorcery::Dialog::set_selected(WindowDialogButton value) -> void {

	_selected = value;
}

auto Sorcery::Dialog::get_selected() -> WindowDialogButton {

	return _selected;
}

auto Sorcery::Dialog::update() -> void {

	const double lerp{_graphics->animation->colour_lerp};
	switch (_type) {
	case WindowDialogType::OK: {
		sf::RectangleShape &hl{_highlights.at(WindowDialogButton::OK)};
		hl.setFillColor(_display->window->change_colour(sf::Color(_buttons_c.background), lerp));
	} break;
	case WindowDialogType::CONFIRM: {
		if (_selected == WindowDialogButton::YES) {

			sf::RectangleShape &hl{_highlights.at(WindowDialogButton::YES)};
			hl.setFillColor(
				_display->window->change_colour(sf::Color(_buttons_c.background), lerp));

		} else if (_selected == WindowDialogButton::NO) {

			sf::RectangleShape &hl{_highlights.at(WindowDialogButton::NO)};
			hl.setFillColor(
				_display->window->change_colour(sf::Color(_buttons_c.background), lerp));
		}
	} break;
	case WindowDialogType::MENU:
		break;
	case WindowDialogType::TIMED:
		break;
	default:
		break;
	}
}

auto Sorcery::Dialog::draw(sf::RenderTarget &target, sf::RenderStates state) const -> void {

	state.transform *= getTransform();
	target.draw(*_frame, state);
	for (auto text : _texts)
		target.draw(text, state);

	switch (_type) {
	case WindowDialogType::OK:
		target.draw(_buttons_hl.at(WindowDialogButton::OK), state);
		break;
	case WindowDialogType::CONFIRM:
		if (_selected == WindowDialogButton::YES) {
			target.draw(_buttons.at(WindowDialogButton::NO), state);
			target.draw(_highlights.at(WindowDialogButton::YES), state);
			target.draw(_buttons_hl.at(WindowDialogButton::YES), state);
		} else if (_selected == WindowDialogButton::NO) {
			target.draw(_buttons.at(WindowDialogButton::YES), state);
			target.draw(_highlights.at(WindowDialogButton::NO), state);
			target.draw(_buttons_hl.at(WindowDialogButton::NO), state);
		}
		break;
	case WindowDialogType::MENU:
		break;
	case WindowDialogType::TIMED:
		break;
	default:
		break;
	}
}