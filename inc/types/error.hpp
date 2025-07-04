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

#pragma once

#include "common/define.hpp"
#include "types/include.hpp"

namespace Sorcery {

class Error {

	public:
		// Constructors
		Error(Enums::System::Error error_code, std::exception &exception,
			  std::string notes = "");
		Error() = delete;

		// Overloaded Operators
		auto friend operator<<(std::ostream &out_stream, const Error &error)
			-> std::ostream &;

		// Public Members

		// Public Methods
		auto get_code() -> Enums::System::Error;
		auto get_exception() -> std::exception &;
		auto get_what() -> std::string;
		auto get_when() -> std::string;
		auto get_notes() -> std::string;
		auto get() -> std::vector<std::string>;

	private:
		// Private Methods

		// Private Members
		Enums::System::Error _error_code;
		std::exception _exception;
		std::chrono::time_point<std::chrono::system_clock> _timestamp;
		std::vector<std::string> _details;
		std::string _notes;
};

}
