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

namespace Sorcery {

// Macro to convert std::string to C string
inline auto CSTR(const std::string &string) -> const char * {

	return !string.empty() ? (char *)string.c_str() : (char *)"";
}

// Timepoint to String
inline auto TP2STR(const TimePoint tp) -> std::string {

	// Need to do it this way til std::chrono::locate_zone etc is supported
	auto t{std::chrono::system_clock::to_time_t(tp)};
	std::string ts{std::ctime(&t)};
	ts.resize(ts.size() - 1);
	return ts;
}

}