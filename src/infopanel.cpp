// Copyright (C) 2021 Dave Moore
//
// This file is part of Sorcery: Dreams of the Mad Overlord.
//
// Sorcery: Dreams of the Mad Overlord is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Sorcery: Dreams of the Mad Overlord is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sorcery: Dreams of the Mad Overlord.  If not, see <http://www.gnu.org/licenses/>.
//
// If you modify this Program, or any covered work, by linking or combining it
// with the libraries referred to in README (or a modified version of said
// libraries), containing parts covered by the terms of said libraries, the
// licensors of this Program grant you additional permission to convey the
// resulting work.

#include "infopanel.hpp"

// Standard Constructor
Sorcery::InfoPanel::InfoPanel(System &system, Display &display, Graphics &graphics)
	: _system{system}, _display{display}, _graphics{graphics} {

	// Get the standard layout information
	_layout = Component((*_display.layout)["global:info_panel"]);

	// Not valid until we call the set command
	valid = false;
}

// Set the Icon
auto Sorcery::InfoPanel::set_icon(const std::string &string) -> void {

	if ((*_graphics.icons)[string])
		_icon = (*_graphics.icons)[string].value();
	else
		_icon = sf::Sprite();

	_icon.setPosition(0, 16);
}

// Set the Text
auto Sorcery::InfoPanel::set_text(const std::string &string) -> void {

	_strings.clear();
	_texts.clear();
	_width = 0;
	_height = 0;

	// Get the display lines
	const std::regex regex(R"([#]+)");
	std::sregex_token_iterator it{string.begin(), string.end(), regex, -1};
	std::vector<std::string> split{it, {}};
	split.erase(std::remove_if(split.begin(), split.end(),
					[](std::string const &s) {
						return s.size() == 0;
					}),
		split.end());
	_strings = split;

	int x{140};
	int y{0};
	for (const auto &each_string : _strings) {
		sf::Text text;
		text.setFont(_system.resources->fonts[_layout.font]);
		text.setCharacterSize(_layout.size);
		text.setFillColor(sf::Color(_layout.colour));
		text.setString(each_string);
		text.setPosition(x, 18 + y * 24);
		++y;
		_texts.push_back(text);
	}

	// Workout the size of the panel
	size_t max_length{0};
	for (const auto &each_string : _strings) {
		if (each_string.length() > max_length) {
			max_length = each_string.length();
		}
	}
	_width = max_length + 4;
	_height = _strings.size() + 4;

	// We're ok to draw it now
	valid = true;
}

auto Sorcery::InfoPanel::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void {

	states.transform *= getTransform();
	for (auto each_text : _texts) {
		target.draw(each_text, states);
	}

	target.draw(_icon, states);
}