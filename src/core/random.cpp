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

#include "core/random.hpp"

Sorcery::Random::Random() {

	// Random Seeding Device
	_random = std::mt19937_64(_device());

	// Populate Ranges
	using enum Enums::System::Random;
	_range.clear();
	_range[D2] = std::make_tuple(1, 2);
	_range[D3] = std::make_tuple(1, 3);
	_range[D4] = std::make_tuple(1, 4);
	_range[D5] = std::make_tuple(1, 5);
	_range[D6] = std::make_tuple(1, 6);
	_range[D7] = std::make_tuple(1, 7);
	_range[D8] = std::make_tuple(1, 8);
	_range[D10] = std::make_tuple(1, 10);
	_range[D15] = std::make_tuple(1, 15);
	_range[D16] = std::make_tuple(1, 16);
	_range[D52] = std::make_tuple(1, 52);
	_range[D100] = std::make_tuple(1, 100);
	_range[D165] = std::make_tuple(1, 165);
	_range[D300] = std::make_tuple(1, 300);
	_range[D1000] = std::make_tuple(1, 1000);
	_range[D2000] = std::make_tuple(1, 2000);
	_range[ZERO_TO_2] = std::make_tuple(0, 2);
	_range[ZERO_TO_3] = std::make_tuple(0, 3);
	_range[ZERO_TO_4] = std::make_tuple(0, 4);
	_range[ZERO_TO_7] = std::make_tuple(0, 7);
	_range[ZERO_TO_8] = std::make_tuple(0, 8);
	_range[ZERO_TO_14] = std::make_tuple(0, 14);
	_range[ZERO_TO_19] = std::make_tuple(0, 19);
	_range[ZERO_TO_29] = std::make_tuple(0, 29);
	_range[ZERO_TO_34] = std::make_tuple(0, 34);
	_range[ZERO_TO_69] = std::make_tuple(0, 69);
	_range[ZERO_TO_99] = std::make_tuple(0, 99);
	_range[ZERO_TO_100] = std::make_tuple(0, 100);
	_range[ZERO_TO_101] = std::make_tuple(0, 101);
	_range[ZERO_TO_129] = std::make_tuple(0, 129);
	_range[ZERO_TO_299] = std::make_tuple(0, 299);
	_range[ZERO_TO_399] = std::make_tuple(0, 399);
	_range[ZERO_TO_437] = std::make_tuple(0, 437);

	// Set up the Random Syllables
	_sy1 = {"Ab",  "Ac", "Ad",	"Af",  "Agr", "Ast",  "As",	 "Al",	"Adw",
			"Adr", "Ar", "B",	"Br",  "C",	  "Cr",	  "Ch",	 "Cad", "D",
			"Dr",  "Dw", "Ed",	"Eth", "Et",  "Er",	  "El",	 "Eow", "F",
			"Fr",  "G",	 "Gr",	"Gw",  "Gal", "Gl",	  "H",	 "Ha",	"Ib",
			"Jer", "K",	 "Ka",	"Ked", "L",	  "Loth", "Lar", "Leg", "M",
			"Mir", "N",	 "Nyd", "Ol",  "Oc",  "On",	  "P",	 "Pr",	"R",
			"Rh",  "S",	 "Sev", "T",   "Tr",  "Th",	  "V",	 "Y",	"Z",
			"W",   "Wic"};
	_sy2 = {"a", "ae", "au", "ao",	"are", "ale", "ali", "ay",	 "ardo",
			"e", "ei", "ea", "eri", "era", "ela", "eli", "enda", "erra",
			"i", "ia", "ie", "ire", "ira", "ila", "ili", "ira",	 "igo",
			"o", "oa", "oi", "oe",	"ore", "u",	  "y"};
	_sy3 = {"a",	 "and",	 "b",	 "bwyn", "baen", "bard", "c",	  "ctred",
			"cred",	 "ch",	 "can",	 "d",	 "dan",	 "don",	 "der",	  "dric",
			"dfrid", "dus",	 "f",	 "g",	 "gord", "gan",	 "l",	  "li",
			"lgrin", "lin",	 "lith", "lath", "loth", "ld",	 "ldric", "ldan",
			"m",	 "mas",	 "mos",	 "mar",	 "mond", "n",	 "nydd",  "nidd",
			"nnon",	 "nwan", "nyth", "nad",	 "nn",	 "nnor", "nd",	  "p",
			"r",	 "ron",	 "rd",	 "s",	 "sh",	 "seth", "sean",  "t",
			"th",	 "tha",	 "tlan", "trem", "tram", "v",	 "vudd",  "w",
			"wan",	 "win",	 "wyn",	 "wyr",	 "wyr",	 "wyth"};
}

auto Sorcery::Random::operator[](const Enums::System::Random random_type)
	-> unsigned int {

	// Initialise ImGUI to use SDL2/OpenGL
	return _get(random_type);
}

auto Sorcery::Random::get_type(const int num) const -> Enums::System::Random {

	return magic_enum::enum_cast<Enums::System::Random>(num).value_or(
		Enums::System::Random::NO_DICE);
}

auto Sorcery::Random::get_random_name() -> std::string {

	auto sy1_dist{
		std::uniform_int_distribution<unsigned int>(0, _sy1.size() - 1)};
	auto sy2_dist{
		std::uniform_int_distribution<unsigned int>(0, _sy2.size() - 1)};
	auto sy3_dist{
		std::uniform_int_distribution<unsigned int>(0, _sy3.size() - 1)};

	auto name{std::format("{}{}{}", _sy1.at(sy1_dist(_random)),
						  _sy2.at(sy2_dist(_random)),
						  _sy3.at(sy3_dist(_random)))};

	return name;
}

auto Sorcery::Random::_get(const Enums::System::Random random_type)
	-> unsigned int {

	// Generate a random number of a specified type
	const auto [min, max]{_range[random_type]};
	auto dist{std::uniform_int_distribution<unsigned int>(min, max)};
	return dist(_random);
}

auto Sorcery::Random::get(const unsigned int min, const unsigned int max)
	-> unsigned int {

	// Generate a random number of a specified range
	auto dist{std::uniform_int_distribution<unsigned int>(min, max)};
	return dist(_random);
}