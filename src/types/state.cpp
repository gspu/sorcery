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

#include "types/state.hpp"
#include "core/system.hpp"
#include "resources/itemstore.hpp"

// Constructor used by Cereal to serialise this item
Sorcery::State::State() {}

// Normal Constructor
Sorcery::State::State(System *system)
	: _system{system} {

	_clear();
	_restart_expedition();
}

auto Sorcery::State::reset_shop(ItemStore *itemstore) -> void {

	for (int id = 0; id < 101; id++) {
		const auto item_type{
			(*itemstore)[magic_enum::enum_cast<Enums::Items::TypeID>(id)
							 .value()]};
		_shop[id] = {item_type.get_shop_inital_stock(),
					 item_type.get_shop_inital_stock(), item_type.get_buy(),
					 item_type.get_sell()};
	}
}

auto Sorcery::State::print() -> void {

	auto text{"State:\n\n"s};

	text.append(std::format("{}\n", _version));
	text.append(std::format("{}\n", _player_depth));
	text.append(std::format("{}\\{}\n", _player_pos.x, _player_pos.y));
	text.append(std::format("{}\n", (int)_playing_facing));
	text.append(std::format("{}\n", _lit));
	text.append(std::format("{}\n\n[", _turns));

	for (const auto id : _party) {
		auto line{std::format("{},", id)};
		text.append(line);
	}
	text.append("]\n");
	std::println("{}", text);
}

auto Sorcery::State::_clear() -> void {

	_party.clear();
	if (level.get()) {
		level.release();
		level.reset();
	}
	level = std::make_unique<Level>();
	_clear_explored();
	_version = 1;
	_turns = 0;

	_log.clear();

	std::fill(quest_item_flags.begin(), quest_item_flags.end(), false);
}

auto Sorcery::State::get_turns() const -> unsigned int {

	return _turns;
}

auto Sorcery::State::pass_turn(unsigned int turns) -> void {

	_turns += turns;
}

auto Sorcery::State::_clear_explored() -> void {

	// TODO: handle different depths etc
	for (int depth = -1; depth >= -10; depth--)
		explored.try_emplace(depth, Explore{});
}

auto Sorcery::State::add_character_by_id(unsigned int char_id) -> bool {

	if (_party.size() < 6) {
		_party.push_back(char_id);
		return true;
	} else
		return false;
}

auto Sorcery::State::reorder_party(std::vector<unsigned int> &new_order)
	-> void {

	_party.clear();
	for (auto char_id : new_order)
		_party.push_back(char_id);
}

auto Sorcery::State::restart_expedition() -> void {

	_restart_expedition();
}

auto Sorcery::State::set_lit(bool lit) -> void {

	_lit = lit;
}

auto Sorcery::State::get_lit() const -> bool {

	return _lit;
}

auto Sorcery::State::set_player_prev_depth(int depth) -> void {

	_previous_depth = depth;
}

auto Sorcery::State::set_depth(int depth) -> void {

	_player_depth = depth;
}

auto Sorcery::State::get_depth() const -> int {

	return _player_depth;
}

auto Sorcery::State::get_player_prev_depth() const -> int {

	return _previous_depth;
}

auto Sorcery::State::_restart_expedition() -> void {

	_playing_facing = Enums::Map::Direction::NORTH;
	_player_pos = Coordinate{0, 0};
	_player_depth = -1;
	_lit = false;
}

// Method called to simulate Normal Constructor with Cereal Constructor
auto Sorcery::State::set(System *system) -> void {

	_system = system;
}

auto Sorcery::State::set_party(std::vector<unsigned int> candidate_party)
	-> void {

	_party = candidate_party;
}

auto Sorcery::State::party_has_members() const -> bool {

	return _party.size() > 0;
}

auto Sorcery::State::get_party_size() const -> unsigned int {

	return _party.size();
}

auto Sorcery::State::get_party_characters() const -> std::vector<unsigned int> {

	return _party;
}

auto Sorcery::State::clear_party() -> void {

	_party.clear();
}

auto Sorcery::State::add_character_to_party(unsigned int char_id) -> bool {

	if (_party.size() < 6) {
		_party.push_back(char_id);
		return true;
	} else
		return false;
}

auto Sorcery::State::check_character_in_party(unsigned int char_id) -> bool {

	if (_party.size() > 0) {
		const auto found{
			std::find_if(_party.begin(), _party.end(), [&](unsigned int id) {
				return id = char_id;
			})};
		return found != std::end(_party);
	} else
		return false;
}

auto Sorcery::State::set_current_level(Level *other) -> void {

	level->set(other);
}

// Return the *slot* of a character in the party (note this is 1-indexed!)
auto Sorcery::State::get_char_slot(unsigned int char_id)
	-> std::optional<unsigned int> {

	if (_party.size() > 0) {

		const auto distance{
			std::find_if(_party.begin(), _party.end(), [&](unsigned int id) {
				return id == char_id;
			})};
		if (distance != _party.end())
			return (std::distance(_party.begin(), distance)) + 1;
		else
			return std::nullopt;
	} else
		return std::nullopt;
}

auto Sorcery::State::get_player_facing() const -> Enums::Map::Direction {

	return _playing_facing;
}

auto Sorcery::State::get_player_pos() const -> Coordinate {

	return _player_pos;
}

auto Sorcery::State::get_player_prev_pos() const -> Coordinate {

	return _previous_pos;
}

auto Sorcery::State::set_player_facing(const Enums::Map::Direction direction)
	-> void {

	_playing_facing = direction;
}

auto Sorcery::State::set_player_pos(const Coordinate position) -> void {

	_previous_pos = _player_pos;
	_player_pos = position;
}

auto Sorcery::State::remove_character_by_id(unsigned int char_id) -> bool {

	if (_party.size() > 0) {
		_party.erase(std::remove_if(_party.begin(), _party.end(),
									[&](unsigned int id) {
										return id == char_id;
									}),
					 _party.end());
		return true;
	} else
		return false;
}

// 1-indexed!
auto Sorcery::State::get_party_char(unsigned int index)
	-> std::optional<unsigned int> {

	if (_party.size() < index)
		return std::nullopt;
	else
		return _party.at(index - 1);
}

auto Sorcery::State::get_next_party_character(unsigned int character_id)
	-> std::optional<unsigned int> {

	auto index{get_char_slot(character_id)};
	if (index) {
		auto val{index.value()};
		return val == _party.size() ? 1 : val++;
	} else
		return character_id;
}

auto Sorcery::State::get_previous_party_character(unsigned int character_id)
	-> std::optional<unsigned int> {

	auto index{get_char_slot(character_id)};
	if (index) {
		auto val{index.value()};
		return val == 1 ? _party.size() : val--;
	} else
		return character_id;
}

auto Sorcery::State::remove_character_by_position(unsigned int index) -> bool {

	if (_party.size() > index) {
		_party.erase(_party.begin() + index);
		return true;
	} else
		return false;
}

auto Sorcery::State::add_log_message(
	std::string text,
	Enums::Internal::MessageType type = Enums::Internal::MessageType::STANDARD)
	-> void {

	_log.emplace_back(ConsoleMessage{type, text});
}

auto Sorcery::State::clear_log_messages() -> void {

	_log.clear();
}

auto Sorcery::State::add_log_dice_roll(const std::string &message,
									   const int dice, const int roll,
									   const int needed) -> void {

	if (dice != -1 || roll != -1 || needed != -1) {
		const auto success{roll < needed ? "SUCCESS" : "FAILURE"};
		const auto string{std::format("{} ({})", message, success)};
		add_log_message(_system->dice_roll_to_str(string, dice, roll, needed),
						Enums::Internal::MessageType::ROLL);
	} else
		add_log_message(message, Enums::Internal::MessageType::GAME);
}

auto Sorcery::State::get_log_messages(unsigned int last) const
	-> std::vector<ConsoleMessage> {

	if (last == 0)
		return _log;
	else {
		if (_log.size() < last)
			last = _log.size();

		std::vector<ConsoleMessage> results{_log.cend() - last, _log.cend()};
		return results;
	}
}

auto Sorcery::State::check_shop_stock(
	const Enums::Items::TypeID item_type) const -> int {

	return _shop[unenum(item_type)].current_stock;
}

auto Sorcery::State::check_shop_will_sell(
	const Enums::Items::TypeID item_type) const -> bool {

	return _shop[unenum(item_type)].sellable;
}

auto Sorcery::State::check_shop_will_buy(
	const Enums::Items::TypeID item_type) const -> bool {

	return _shop[unenum(item_type)].buyable;
}

auto Sorcery::State::sell_to_shop(ItemStore *itemstore,
								  const Enums::Items::TypeID item_type) -> int {

	if (_shop[unenum(item_type)].current_stock != -1) {
		++_shop[unenum(item_type)].current_stock;
		return (*itemstore)[item_type].get_value() / 2;
	} else
		return 0;
}

auto Sorcery::State::buy_from_shop(ItemStore *itemstore,
								   const Enums::Items::TypeID item_type)
	-> int {

	if (_shop[unenum(item_type)].current_stock > 0) {
		--_shop[unenum(item_type)].current_stock;
		return 0 - (*itemstore)[item_type].get_value();
	} else
		return 0;
}

auto Sorcery::State::get_shop_display(ItemStore *itemstore,
									  const Enums::Items::TypeID item_type)
	-> std::string {

	const auto item{(*itemstore)[item_type]};
	const std::string flag{std::invoke([&] {
		if (_shop[unenum(item_type)].current_stock == -1)
			return std::string{"(*)"};
		else
			return std::format("({})", _shop[unenum(item_type)].current_stock);
	})};

	const std::string line{std::format("{:>16} {:<5} {:>7} GP",
									   item.get_display_name(), flag,
									   item.get_value())};

	return line;
}
