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

#include "types/dice.hpp"

std::random_device Sorcery::Dice::_device;
std::mt19937_64 Sorcery::Dice::_random(_device());

Sorcery::Dice::Dice() {

	num = 0;
	dice = 0;
	mod = 0;
}

Sorcery::Dice::Dice(const unsigned int num_, const unsigned int dice_)
	: num{num_},
	  dice{dice_},
	  mod{0} {}

Sorcery::Dice::Dice(const std::string dice_) {

	if (dice_.length() > 0) {
		const std::regex regex(R"(^(\d+)d(\d+)[+-]?(\d*)$)");
		if (std::smatch match; std::regex_search(dice_, match, regex)) {
			num = std::stoi(match[1]);
			dice = std::stoi(match[2]);
			if (match[3].length() > 0)
				mod = std::stoi(match[3]);
			else
				mod = 0;
			if (dice_.find('-') != std::string::npos)
				mod = -mod;
		} else {
			num = 0;
			dice = 0;
			mod = 0;
		}
	} else {

		num = 0;
		dice = 0;
		mod = 0;
	}
}

Sorcery::Dice::Dice(const unsigned int num_, const unsigned int dice_,
					const int mod_)
	: num{num_},
	  dice{dice_},
	  mod{mod_} {}

auto Sorcery::Dice::roll() const -> int {

	if (dice > 0) {
		auto dist{std::uniform_int_distribution<unsigned int>(1, dice)};
		return num * dist(_random) + mod;
	} else
		return 0;
}

auto Sorcery::Dice::roll_min() const -> int {

	return dice + mod;
}

auto Sorcery::Dice::roll_max() const -> int {

	return (num * dice) + mod;
}

auto Sorcery::Dice::str() const -> std::string {

	std::string result;
	if (mod == 0)
		result = std::format("{}d{}", num, dice);
	else if (mod < 0)
		result = std::format("{}d{}{}", num, dice, mod);
	else
		result = std::format("{}d{}+{}", num, dice, mod);

	return result;
}

auto Sorcery::Dice::set(const unsigned int num_, const unsigned int dice_,
						const int mod_) {

	num = num_;
	dice = dice_;
	mod = mod_;
}
