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

#pragma once

#include "common/include.hpp"
#include "core/include.hpp"
#include "core/window.hpp"
#include "gui/controloverlay.hpp"
#include "resources/componentstore.hpp"
#include "resources/iconstore.hpp"
#include "resources/stringstore.hpp"
#include "types/component.hpp"

namespace Sorcery {

// Forward Declarations
class System;
class Frame;

// Superclass to handle basic display requirements such as Windopw Access, Game
// Text and so on
class Display {

	public:

		// Constructors
		Display(System *system);
		Display() = delete;

		// Public Methods
		auto generate(std::string_view screen) -> void;
		auto display(std::string_view screen,
			std::optional<std::any> param = std::nullopt) -> void;
		auto display(std::string_view screen, SpriteMap &sprites,
			TextMap &texts, FrameMap &frames,
			std::optional<std::any> param = std::nullopt) -> void;
		auto generate(std::string_view screen, SpriteMap &sprites,
			TextMap &texts, FrameMap &frames) -> void;
		auto display_cursor() -> void;
		auto fit_bg_movie() -> void;
		auto start_bg_movie() -> void;
		auto stop_bg_movie() -> void;
		auto update_bg_movie() -> void;
		auto draw_bg_movie() -> void;
		auto set_input_mode(WIM input_mode) -> void;
		auto get_input_mode() const -> WIM;
		auto show_overlay() -> void;
		auto overlay_shown() const -> bool;
		auto hide_overlay() -> void;
		auto display_overlay() -> void;
		auto shutdown_SFML() -> void;
		auto display_direction_indicator(MAD dir, bool mono) -> void;
		auto get_bold() const -> bool;
		auto get_upper() const -> bool;
		auto get_centre_pos(const sf::Vector2f size) const -> sf::Vector2f;
		auto get_centre_x(const unsigned int width) const -> unsigned int;
		auto get_centre_y(const unsigned int height) const -> unsigned int;
		auto get_disc() const -> bool;
		auto set_disc(const bool value) -> void;
		auto get_cur() const -> sf::Vector2f;

		// Public Members
		std::unique_ptr<StringStore> string;
		std::unique_ptr<Window> window;
		std::unique_ptr<ComponentStore> layout;
		std::unique_ptr<ControlOverlay> overlay;

	private:

		// Private Members
		SpriteMap _sprites;
		TextMap _texts;
		FrameMap _frames;
		System *_system;
		sfe::Movie _bg_movie;
		ComponentList _components;
		std::unique_ptr<IconStore> _icons;
		bool _show_overlay;
		unsigned int _wp_idx;
		bool _bold_text;
		bool _upper_text;
		bool _disc_access;

		// Private Methods
};

}
