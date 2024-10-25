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

#include "gui/iconpanel.hpp"
#include "core/animation.hpp"
#include "core/display.hpp"
#include "core/game.hpp"
#include "core/graphics.hpp"
#include "core/system.hpp"
#include "resources/componentstore.hpp"
#include "resources/factory.hpp"
#include "resources/iconstore.hpp"

Sorcery::IconPanel::IconPanel(System *system, Display *display,
	Graphics *graphics, Game *game, Component layout, bool is_left)
	: _system{system}, _display{display}, _graphics{graphics}, _game{game},
	  _layout{layout}, _is_left{is_left} {

	_sprites.clear();
	_texts.clear();
	_icons.clear();

	// Setup the Factory
	_factory = std::make_unique<Factory>(_system, _display, _graphics, _game);

	// Make the Frame
	_frame = _factory->make_comp_frame(_layout, _sprites);

	if (_is_left) {
		_add_icon("achievements", "left_icon_panel:achievements");
		_add_icon("items", "left_icon_panel:items");
		_add_icon("talk", "left_icon_panel:talk");
		_add_icon("kick", "left_icon_panel:kick");
		_add_icon("reorder", "left_icon_panel:reorder");
		_add_icon("controls", "left_icon_panel:controls");
		_add_icon("options", "left_icon_panel:options");
		_add_icon("save", "left_icon_panel:save");
		_add_icon("exit", "left_icon_panel:exit");
	} else {
		_add_icon("direction", "right_icon_panel:forward");
		_add_icon("direction", "right_icon_panel:turn_left");
		_add_icon("field", "right_icon_panel:camp");
		_add_icon("direction", "right_icon_panel:turn_right");
		_add_icon("direction", "right_icon_panel:backward");
		_add_icon("party", "right_icon_panel:party");
		_add_icon("magic", "right_icon_panel:magic");
		_add_icon("use", "right_icon_panel:use");
		_add_icon("examine", "right_icon_panel:examine");
	}
}

auto Sorcery::IconPanel::_add_icon(
	std::string_view icon_key, std::string_view component_key) -> void {

	auto offset_x{std::stoi(_layout["offset_x"].value())};
	auto offset_y{std::stoi(_layout["offset_y"].value())};

	auto icon{(*_graphics->icons)[icon_key].value()};
	auto component{(*_display->layout)[component_key]};
	_set_icon(icon, component, offset_x, offset_y);
	_icons.emplace_back(std::make_pair(component_key, icon));
}

auto Sorcery::IconPanel::_set_icon(
	sf::Sprite &sprite, Component layout, int offset_x, int offset_y) -> void {

	sprite.setOrigin(
		sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
	sprite.rotate(std::stof(layout["rotation"].value()));
	sprite.setScale(layout.scl());
	sprite.setColor(sf::Color(layout.colour));
	const auto comp_offset_x{std::invoke([&] {
		if (layout["offset_x"])
			return std::stoi(layout["offset_x"].value());
		else
			return 0;
	})};
	const auto comp_offset_y{std::invoke([&] {
		if (layout["offset_y"])
			return std::stoi(layout["offset_y"].value());
		else
			return 0;
	})};

	sprite.setPosition(layout.x + offset_x + comp_offset_x,
		layout.y + offset_y + comp_offset_y);
}

auto Sorcery::IconPanel::refresh(bool in_camp) -> void {

	if (in_camp) {

	} else {
	}
}

auto Sorcery::IconPanel::set_mouse_selected(Component &component,
	sf::Vector2f mouse_pos) -> std::optional<std::string> {

	// Now look through the global positions of each icon and see if it matches
	// the mouse position
	for (auto &[key, sprite] : _icons) {
		sf::Rect sprite_area{sprite.getGlobalBounds()};
		sprite_area.left += component.x;
		sprite_area.top += component.y;
		if (sprite_area.contains(mouse_pos))
			return key;
	}

	return std::nullopt;
}

auto Sorcery::IconPanel::is_mouse_over(
	Component &component, sf::Vector2f mouse_pos) const -> bool {

	// Now look through the global positions of each icon and see if it matches
	// the mouse position
	for (auto &[key, sprite] : _icons) {
		sf::Rect sprite_area{sprite.getGlobalBounds()};
		sprite_area.left += component.x;
		sprite_area.top += component.y;
		if (sprite_area.contains(mouse_pos))
			return true;
	}

	return false;
}

auto Sorcery::IconPanel::set_selected_background() -> void {

	if (selected) {

		// Find the text that is highlighted
		IconStorage::const_iterator it{
			std::find_if(_icons.begin(), _icons.end(),
				[&](auto value) { return value.first == selected.value(); })};
		if (it != _icons.end()) {
			auto &sprite{it->second};

			_selected_bg =
				sf::RectangleShape(sf::Vector2(sprite.getGlobalBounds().width,
					sprite.getGlobalBounds().height));
			_selected_bg.setFillColor(_graphics->animation->select_col);
			_selected_bg.setPosition(
				sprite.getPosition().x - (sprite.getGlobalBounds().width / 2),
				sprite.getPosition().y - (sprite.getGlobalBounds().height / 2));
		}
	}
}

auto Sorcery::IconPanel::draw(
	sf::RenderTarget &target, sf::RenderStates states) const -> void {

	states.transform *= getTransform();

	// Draw the standard components
	for (const auto &sprite : _sprites)
		target.draw(sprite, states);

	if (selected)
		target.draw(_selected_bg, states);

	for (const auto &[key, icon] : _icons)
		target.draw(icon, states);

	for (const auto &text : _texts)
		target.draw(text, states);
}
