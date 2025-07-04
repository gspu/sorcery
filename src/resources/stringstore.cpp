// Copyright (C) 2025 Dave Moore
//
// This file is part of Sorcery.
//
// Sorcery is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 2 of the License, or (at your option) any later
// version.
//
// Sorcery is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// Sorcery.  If not, see <http://www.gnu.org/licenses/>.
//
// If you modify this program, or any covered work, by linking or combining
// it with the libraries referred to in README (or a modified version of
// said libraries), containing parts covered by the terms of said libraries,
// the licensors of this program grant you additional permission to convey
// the resulting work.

#include "resources/stringstore.hpp"
#include "resources/define.hpp"
#include "resources/include.hpp"

Sorcery::StringStore::StringStore(const std::string &filename)
	: _filename{filename} {

	// Load strings from file
	_loaded = _load();
}

auto Sorcery::StringStore::operator[](std::string_view key) -> std::string & {

	return _loaded ? _strings[std::string{key}] : _strings["NONE"];
}

auto Sorcery::StringStore::reload() -> void {

	_loaded = _load();
}

auto Sorcery::StringStore::_load() -> bool {

	// Attempt to load the Strings File
	_strings.clear();
	_strings["NONE"] = STRINGS_NOT_LOADED;
	if (std::ifstream file{_filename, std::ifstream::binary}; file.good()) {

		// Iterate through the file
		Json::Value root{};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		Json::Reader reader{};
#pragma GCC diagnostic pop
		Json::StreamWriterBuilder builder{};
		builder.settings_["indentation"] = "";
		if (reader.parse(file, root, false)) {
			for (Json::Value::iterator it = root.begin(); it != root.end();
				 ++it) {
				Json::Value key{it.key()};
				Json::Value value{*it};
				auto string_key{Json::writeString(builder, key)};
				auto string_value{Json::writeString(builder, value)};

				// Remove any Special Characters from the string
				string_key.erase(
					remove(string_key.begin(), string_key.end(), '\"'),
					string_key.end());
				string_value.erase(
					remove(string_value.begin(), string_value.end(), '\"'),
					string_value.end());
				string_key.erase(
					remove(string_key.begin(), string_key.end(), '\n'),
					string_key.end());

				// Insert it into the map
				_strings[string_key] = string_value;
			}
		} else
			return false;
	} else
		return false;

	return true;
}

auto Sorcery::StringStore::get(std::string_view key) -> std::string {

	if (_loaded)
		return _strings.contains(std::string{key})
				   ? _strings.at(std::string{key})
				   : KEY_NOT_FOUND;
	else
		return STRINGS_NOT_LOADED;
}