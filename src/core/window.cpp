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

#include "core/window.hpp"
#include "core/system.hpp"
#include "resources/componentstore.hpp"
#include "resources/stringstore.hpp"

// Standard Constructor
Sorcery::Window::Window(System *system, StringStore *string,
	ComponentStore *layout, const std::string &title)
	: _system{system}, _string{string}, _layout{layout}, _title{title} {

	// First get the Window Size from System Config
	_default_size.w =
		std::stoi(_system->config->get("Window", DEFAULT_SCREEN_WIDTH));
	_default_size.h =
		std::stoi(_system->config->get("Window", DEFAULT_SCREEN_HEIGHT));
	_current_size.w =
		std::stoi(_system->config->get("Window", CURRENT_SCREEN_WIDTH));
	_current_size.h =
		std::stoi(_system->config->get("Window", CURRENT_SCREEN_HEIGHT));

	// And then the Positioning Grid Cell Size
	_cell_height = std::stoi(_system->config->get("Grid", CELL_HEIGHT));
	_cell_width = std::stoi(_system->config->get("Grid", CELL_WIDTH));
	_layout->set_grid(_cell_width, _cell_height);

#ifdef linux
	XInitThreads();
#endif

	// Then create the SFML Window and the Main GUI
	_window.create(sf::VideoMode(_current_size.w, _current_size.h), _title,
		sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
	_window.setFramerateLimit(FRAMERATE);
	centre.x = _current_size.w / 2;
	centre.y = _current_size.h / 2;
	_window.setVerticalSyncEnabled(true);
	_gui.setTarget(_window);
	_theme = tgui::Theme((*system->files)[GUI_FILE].string());
	tgui::Theme::setDefault((*system->files)[GUI_FILE].string());

	view = _window.getView();

	// Change the Mouse Cursor
	_window.setMouseCursorVisible(false);
	size = sf::IntRect(0, 0, _current_size.w, _current_size.h);

	// Screenshot Capture Buffer
	_capture.create(
		_window.getSize().x, _window.getSize().y, sf::Color(0x000000ff));
	_texture.create(_window.getSize().x, _window.getSize().y);

	// Default for Cursor Coordinates
	_show_cursor_coords = false;
	_cursor_coord = sf::Text{};
}

auto Sorcery::Window::clear_window() -> void {

	_window.clear();
}

auto Sorcery::Window::display_window() -> void {

	_window.display();
}

auto Sorcery::Window::save_screen() -> void {

	_texture.update(_window);
	_capture = _texture.copyToImage();
}

auto Sorcery::Window::restore_screen() -> void {

	_texture.loadFromImage(_capture);
	sf::Sprite sprite{};
	sprite.setPosition(0, 0);
	sprite.setTexture(_texture, true);
	_window.draw(sprite);
}

auto Sorcery::Window::draw_gui() -> void {

	_gui.draw();
}

// Draw Text on the Screen
auto Sorcery::Window::draw_text(sf::Text &text) -> void {

	_draw_text(text);
}

auto Sorcery::Window::draw_text(
	sf::Text &text, const Component &comp, const std::string &string) -> void {

	_draw_text(text, comp, string);
}

auto Sorcery::Window::draw_text(sf::Text &text, const Component &comp,
	const std::string &string, const double lerp) -> void {

	_draw_text(text, comp, string, lerp);
}

auto Sorcery::Window::draw_text(
	sf::Text &text, const Component &comp, const double lerp) -> void {

	_draw_text(text, comp, lerp);
}

auto Sorcery::Window::_draw_text(sf::Text &text) -> void {

	_window.draw(text);
}

auto Sorcery::Window::_draw_text(
	sf::Text &text, const Component &comp, const double lerp) -> void {

	text.setFont(_system->resources->fonts[comp.font]);
	text.setCharacterSize(comp.size);
	if (comp.animated)
		text.setFillColor(
			sf::Color(_adjust_brightness(sf::Color(comp.colour), lerp)));
	else
		text.setFillColor(sf::Color(comp.colour));
	text.setString((*_string)[comp.string_key]);
	if (_bold_text)
		text.setStyle(sf::Text::Bold);

	const auto x{comp.x == -1 ? centre.x : comp.x};
	const auto y{comp.y == -1 ? centre.y : comp.y};
	if (comp.justification == JUS::CENTRE) {
		text.setPosition(x, y);
		text.setOrigin(text.getLocalBounds().width / 2.0f, 0);
	} else if (comp.justification == JUS::RIGHT) {
		text.setPosition(x, y);
		const sf::FloatRect bounds{text.getLocalBounds()};
		text.setPosition(comp.x - bounds.width, comp.y);
	} else
		text.setPosition(x, y);

	// Handle varying height of proportional fonts
	if (comp.font == FTT::PROPORTIONAL)
		text.setPosition(text.getPosition().x,
			text.getPosition().y -
				((comp.size - text.getLocalBounds().height) / 2));

	_window.draw(text);
}

auto Sorcery::Window::set_bold(const bool value) -> void {

	_bold_text = value;
}

auto Sorcery::Window::set_upper(const bool value) -> void {

	_upper_text = value;
}

auto Sorcery::Window::_draw_text(
	sf::Text &text, const Component &comp, const std::string &string) -> void {

	text.setFont(_system->resources->fonts[comp.font]);
	text.setCharacterSize(comp.size);
	text.setFillColor(sf::Color(comp.colour));
	text.setString(string);
	if (_bold_text)
		text.setStyle(sf::Text::Bold);

	const auto x{comp.x == -1 ? centre.x : comp.x};
	const auto y{comp.y == -1 ? centre.y : comp.y};
	if (comp.justification == JUS::CENTRE) {
		text.setPosition(x, y);
		text.setOrigin(text.getLocalBounds().width / 2.0f, 0);
	} else if (comp.justification == JUS::RIGHT) {
		text.setPosition(x, y);
		const sf::FloatRect bounds{text.getLocalBounds()};
		text.setPosition(comp.x - bounds.width, comp.y);
	} else
		text.setPosition(x, y);

	// Handle varying height of proportional fonts
	if (comp.font == FTT::PROPORTIONAL)
		text.setPosition(text.getPosition().x,
			text.getPosition().y -
				((comp.size - text.getLocalBounds().height) / 2));

	_window.draw(text);
}

// Shove one text component next to another!
auto Sorcery::Window::shove_text(const sf::Text &shovee, sf::Text &shover,
	unsigned int gap_units) const -> void {

	shover.setPosition(shovee.getGlobalBounds().left +
						   shovee.getGlobalBounds().width +
						   (_cell_width * gap_units),
		shovee.getGlobalBounds().top - (shovee.getGlobalBounds().height / 4));
}

auto Sorcery::Window::get_cur() const -> sf::Vector2f {

	return static_cast<sf::Vector2f>(sf::Mouse::getPosition(_window));
}

auto Sorcery::Window::set_pos(Component *comp, sf::Transformable *object) const
	-> void {

	const auto off_x{std::invoke([&] {
		if ((*comp)["offset_x"])
			return std::stoi((*comp)["offset_x"].value());
		else
			return 0;
	})};
	const auto off_y{std::invoke([&] {
		if ((*comp)["offset_y"])
			return std::stoi((*comp)["offset_y"].value());
		else
			return 0;
	})};

	object->setPosition(comp->x + off_x, comp->y + off_y);
}

auto Sorcery::Window::shove_text(
	const sf::Text &shovee, sf::Text &shover, float gap_units) const -> void {

	shover.setPosition(shovee.getGlobalBounds().left +
						   shovee.getGlobalBounds().width +
						   (_cell_width * gap_units),
		shovee.getGlobalBounds().top - (shovee.getGlobalBounds().height / 2) -
			2);
}

auto Sorcery::Window::get_cursor_coord() const -> bool {

	return _show_cursor_coords;
}

auto Sorcery::Window::_draw_text(sf::Text &text, const Component &comp,
	const std::string &string, const double lerp) -> void {

	text.setFont(_system->resources->fonts[comp.font]);
	text.setCharacterSize(comp.size);
	if (comp.animated)
		text.setFillColor(
			sf::Color(_adjust_brightness(sf::Color(comp.colour), lerp)));
	else
		text.setFillColor(sf::Color(comp.colour));
	text.setString(string);
	if (_bold_text)
		text.setStyle(sf::Text::Bold);

	const auto x{comp.x == -1 ? centre.x : comp.x};
	const auto y{comp.y == -1 ? centre.y : comp.y};
	if (comp.justification == JUS::CENTRE) {
		text.setPosition(x, y);
		text.setOrigin(text.getLocalBounds().width / 2.0f,
			text.getLocalBounds().height / 2.0f);
	} else if (comp.justification == JUS::RIGHT) {
		text.setPosition(x, y);
		const sf::FloatRect bounds{text.getLocalBounds()};
		text.setPosition(comp.x - bounds.width, comp.y);
	} else
		text.setPosition(x, y);

	// Handle varying height of proportional fonts
	if (comp.font == FTT::PROPORTIONAL)
		text.setPosition(text.getPosition().x,
			text.getPosition().y -
				((comp.size - text.getLocalBounds().height) / 2));

	_window.draw(text);
}

auto Sorcery::Window::get_cursor() const -> sf::Sprite {

	sf::Sprite cursor(*_system->resources->get_texture(GTX::UI));
	const sf::IntRect cursor_rect(0, 85, 40, 40);
	cursor.setTextureRect(cursor_rect);

	return cursor;
}

auto Sorcery::Window::_get_cursor_coord_text(
	sf::Vector2f mouse_pos, sf::Text &text) -> void {

	auto str{fmt::format("({}/{})", mouse_pos.x, mouse_pos.y)};

	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Red);
	text.setOutlineThickness(1);
	text.setFont(_system->resources->fonts[FTT::TEXT]);
	text.setCharacterSize(16);
	text.setString(str);
	text.setPosition(1, 1);
}

auto Sorcery::Window::get_disc() const -> sf::Sprite {

	sf::Sprite disc(*_system->resources->get_texture(GTX::UI));
	const sf::IntRect disc_rect(40, 85, 32, 32);
	disc.setTextureRect(disc_rect);

	return disc;
}

auto Sorcery::Window::get_gui() -> tgui::Gui * {

	return &_gui;
}

auto Sorcery::Window::get_window() -> sf::RenderWindow * {

	return &_window;
}

auto Sorcery::Window::get_x(const sf::Sprite &sprite, int x_position) const
	-> unsigned int {

	return _get_x(sprite, x_position);
}

auto Sorcery::Window::get_y(const sf::Sprite &sprite, int y_position) const
	-> unsigned int {

	return _get_y(sprite, y_position);
}

auto Sorcery::Window::get_x(const sf::Text &text, int x_position) const
	-> unsigned int {

	return _get_x(text, x_position);
}

auto Sorcery::Window::get_y(const sf::Text &text, int y_position) const
	-> unsigned int {

	return _get_y(text, y_position);
}

auto Sorcery::Window::get_x(unsigned int width, int x_position) const
	-> unsigned int {

	return _get_x(width, x_position);
}

auto Sorcery::Window::get_y(unsigned int width, int y_position) const
	-> unsigned int {

	return _get_y(width, y_position);
}

auto Sorcery::Window::get_centre_x(unsigned int width) const -> unsigned int {

	return (_current_size.w - width) / 2.0f;
}

auto Sorcery::Window::get_centre_y(unsigned int height) const -> unsigned int {

	return (_current_size.h - height) / 2.0f;
}

auto Sorcery::Window::_get_centre_x(unsigned int width) const -> unsigned int {

	return (_current_size.w - width) / 2.0f;
}

auto Sorcery::Window::_get_centre_x(const sf::Sprite &sprite) const
	-> unsigned int {

	return (_current_size.w - sprite.getGlobalBounds().width) / 2.0f;
}

auto Sorcery::Window::_get_centre_x(const sf::Text &text) const
	-> unsigned int {

	return (_current_size.w - text.getGlobalBounds().width) / 2.0f;
}

auto Sorcery::Window::_get_centre_y(unsigned int height) const -> unsigned int {

	return (_current_size.h - height) / 2.0f;
}

auto Sorcery::Window::_get_centre_y(const sf::Sprite &sprite) const
	-> unsigned int {

	return (_current_size.h - sprite.getGlobalBounds().height) / 2.0f;
}

auto Sorcery::Window::_get_centre_y(const sf::Text &text) const
	-> unsigned int {

	return (_current_size.h - text.getGlobalBounds().height) / 2.0f;
}

auto Sorcery::Window::_get_x(
	const sf::Sprite &sprite, const int x_position) const -> unsigned int {

	return x_position == -1 ? _get_centre_x(sprite) : x_position;
}

auto Sorcery::Window::_get_x(const sf::Text &text, const int x_position) const
	-> unsigned int {

	return x_position == -1 ? _get_centre_x(text) : x_position;
}

auto Sorcery::Window::_get_x(unsigned int width, const int x_position) const
	-> unsigned int {

	return x_position == -1 ? _get_centre_x(width) : x_position;
}

auto Sorcery::Window::_get_y(
	const sf::Sprite &sprite, const int y_position) const -> unsigned int {

	return y_position == -1 ? _get_centre_y(sprite) : y_position;
}

auto Sorcery::Window::_get_y(const sf::Text &text, const int y_position) const
	-> unsigned int {

	return y_position == -1 ? _get_centre_y(text) : y_position;
}

auto Sorcery::Window::_get_y(unsigned int width, const int y_position) const
	-> unsigned int {

	return y_position == -1 ? _get_centre_y(width) : y_position;
}

auto Sorcery::Window::get_ch() const -> unsigned int {

	return _cell_height;
}

auto Sorcery::Window::get_cw() const -> unsigned int {

	return _cell_width;
}

auto Sorcery::Window::hl_text(sf::Text &text, const Component &component,
	const double lerp) -> sf::RectangleShape {

	const sf::FloatRect text_rect{text.getGlobalBounds()};
	sf::RectangleShape text_bg(sf::Vector2(text_rect.width, text_rect.height));
	text_bg.setFillColor(
		sf::Color(_adjust_brightness(sf::Color(component.background), lerp)));
	text.setFillColor(sf::Color(component.colour));
	text.setOutlineColor(sf::Color(0, 0, 0));
	text.setOutlineThickness(2);
	return text_bg;
}

auto Sorcery::Window::set_input_mode(WIM value) -> void {

	_input_mode = value;
}

auto Sorcery::Window::get_input_mode() const -> WIM {

	return _input_mode;
}

auto Sorcery::Window::set_cursor_coord(const bool value) -> void {

	_show_cursor_coords = value;
}

auto Sorcery::Window::draw_cursor_coord(const sf::Vector2f mouse_pos) -> void {

	if (_show_cursor_coords) {
		_get_cursor_coord_text(mouse_pos, _cursor_coord);
		_window.draw(_cursor_coord);
	}
}

auto Sorcery::Window::_adjust_brightness(
	sf::Color colour, double colour_lerp) const -> unsigned long long {

	thor::ColorGradient gradient{};
	gradient[0.0f] = sf::Color(0x404040ff);
	gradient[0.5f] = colour;
	gradient[1.0f] = sf::Color(0xbfbfbfff);

	return (gradient.sampleColor(colour_lerp)).toInteger();
}
