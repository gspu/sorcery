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
#include "resources/define.hpp"
#include "resources/filestore.hpp"
#include "resources/include.hpp"

namespace Sorcery {

// Forward Declarations
class TextFile;

class ResourceManager {

	public:

		// Constructors
		ResourceManager(FileStore &files);

		// Destructors
		~ResourceManager();

		// Public Members
		thor::ResourceHolder<sf::Font, FTT> fonts;
		std::unique_ptr<TextFile> license_file;

		// Public Methods
		auto get_font_height(const FTT font_type, const unsigned int size,
			bool bold = false) const -> unsigned int;
		auto get_texture(const GTX texture_type) -> sf::Texture *;
		auto get_texture_ref(const GTX texture_type) -> sf::Texture &;

	private:

		// Private Methods

		// Private Members
		FileStore _files;
		std::map<GTX, bool> _loaded;
		std::map<GTX, std::string> _map;
		thor::ResourceHolder<sf::Texture, Enums::Graphics::Texture> _textures;
};

}
