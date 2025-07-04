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

#include "types/itemtype.hpp"
#include "common/macro.hpp"

std::random_device Sorcery::ItemType::_device;
std::mt19937_64 Sorcery::ItemType::_random(_device());

auto Sorcery::ItemType::get_type_id() const -> Enums::Items::TypeID {

	return _type;
}

auto Sorcery::ItemType::get_known_name() const -> std::string {

	return _known_name;
}

auto Sorcery::ItemType::get_display_name() const -> std::string {

	return _display_name;
}

auto Sorcery::ItemType::get_unknown_name() const -> std::string {

	return _unknown_name;
}

auto Sorcery::ItemType::get_category() const -> Enums::Items::Category {

	return _category;
}

auto Sorcery::ItemType::get_damage() const -> std::string {

	return _damage_str;
}

auto Sorcery::ItemType::get_cursed() const -> bool {

	return _cursed;
}

auto Sorcery::ItemType::get_value() const -> unsigned int {

	return _value;
}

auto Sorcery::ItemType::set_effects(const std::string value) -> void {

	_effects = value;
}

auto Sorcery::ItemType::get_effects() const -> std::string {

	return _effects;
}

auto Sorcery::ItemType::set_usage(const std::string value) -> void {

	_usage = value;
}
auto Sorcery::ItemType::get_usage() const -> std::string {

	return _usage;
}

auto Sorcery::ItemType::set_invokage(const std::string value) -> void {

	_invokage = value;
}

auto Sorcery::ItemType::get_invokage() const -> std::string {

	return _invokage;
}

auto Sorcery::ItemType::get_sellable() const -> bool {

	return _sellable;
}

auto Sorcery::ItemType::get_usable_class() const -> std::array<bool, 9> {

	return _usable;
}

auto Sorcery::ItemType::get_usable_alignment() const -> std::array<bool, 4> {

	return _alignment;
}

auto Sorcery::ItemType::get_swings() const -> int {

	return _swings;
}

auto Sorcery::ItemType::get_to_hit_mod() const -> int {

	return _to_hit_modifier;
}

auto Sorcery::ItemType::get_damage_dice_number() const -> int {

	return _damage_dice.num;
}

auto Sorcery::ItemType::get_damage_dice_type() const -> int {

	return _damage_dice.dice;
}

auto Sorcery::ItemType::get_random_damage() const -> int {

	return _damage_dice.roll();
}

auto Sorcery::ItemType::get_damage_dice_mod() const -> int {

	return _damage_dice.mod;
}

auto Sorcery::ItemType::get_ac_mod() const -> int {

	return _ac_modifier;
}

auto Sorcery::ItemType::get_curse_ac_mod() const -> int {

	return _curse_ac_modifier;
}

auto Sorcery::ItemType::get_regen() const -> int {

	return _regeneration;
}

auto Sorcery::ItemType::get_eff_def() const -> std::array<bool, 22> {

	return _defensive_effects;
}

auto Sorcery::ItemType::get_eff_off() const -> std::array<bool, 15> {

	return _offensive_effects;
}

auto Sorcery::ItemType::get_eff_inv() const -> Enums::Items::Effects::Invoke {

	return _invocation_effect;
}

auto Sorcery::ItemType::get_eff_inv_decay() const -> unsigned int {

	return _invocation_decay_chance;
}

auto Sorcery::ItemType::get_eff_use() const -> Enums::Magic::SpellID {

	return _use_effect;
}

auto Sorcery::ItemType::get_eff_use_decay() const -> unsigned int {

	return _use_decay_chance;
}

auto Sorcery::ItemType::get_decay_type_id() const -> Enums::Items::TypeID {

	return _decay_type;
}

auto Sorcery::ItemType::get_shop_inital_stock() const -> int {

	return _shop_initial_stock;
}

auto Sorcery::ItemType::get_discovered() const -> bool {

	return _discovered_by_player;
}

auto Sorcery::ItemType::get_desc() const -> std::string {

	return _description;
}

auto Sorcery::ItemType::get_gfx() const -> unsigned int {

	return _gfx;
}

auto Sorcery::ItemType::is_class_usable(
	const Enums::Character::Class cclass) const -> bool {

	auto cc{unenum(cclass)};

	return _usable[cc];
}

auto Sorcery::ItemType::is_align_usable(
	const Enums::Character::Align calign) const -> bool {

	auto ca{unenum(calign)};

	return _alignment[ca];
}

auto Sorcery::ItemType::set_type_id(const Enums::Items::TypeID value) -> void {

	_type = value;
}

auto Sorcery::ItemType::set_known_name(const std::string value) -> void {

	_known_name = value;
}

auto Sorcery::ItemType::set_display_name(const std::string value) -> void {

	_display_name = value;
}

auto Sorcery::ItemType::set_damage(const std::string value) -> void {

	_damage_str = value;
}

auto Sorcery::ItemType::set_damage_dice(const std::string value) -> void {

	_damage_dice = Dice{value};
}

auto Sorcery::ItemType::set_unknown_name(const std::string value) -> void {

	_unknown_name = value;
}

auto Sorcery::ItemType::set_category(const Enums::Items::Category value)
	-> void {

	_category = value;
}

auto Sorcery::ItemType::set_cursed(const bool value) -> void {

	_cursed = value;
}

auto Sorcery::ItemType::set_value(const unsigned int value) -> void {

	_value = value;
}

auto Sorcery::ItemType::set_sellable(const bool value) -> void {

	_sellable = value;
}

auto Sorcery::ItemType::set_usable_class(const std::array<bool, 9> value)
	-> void {

	_usable = value;
}

auto Sorcery::ItemType::set_usable_alignment(const std::array<bool, 4> value)
	-> void {

	_alignment = value;
}

auto Sorcery::ItemType::set_swings(const int value) -> void {

	_swings = value;
}

auto Sorcery::ItemType::set_to_hit_mod(const int value) -> void {

	_to_hit_modifier = value;
}

auto Sorcery::ItemType::set_ac_mod(const int value) -> void {

	_ac_modifier = value;
}

auto Sorcery::ItemType::set_curse_ac_mod(const int value) -> void {

	_curse_ac_modifier = value;
}

auto Sorcery::ItemType::set_regen(const int value) -> void {

	_regeneration = value;
}

auto Sorcery::ItemType::set_eff_def(const std::array<bool, 22> value) -> void {

	_defensive_effects = value;
}

auto Sorcery::ItemType::set_eff_off(const std::array<bool, 15> value) -> void {

	_offensive_effects = value;
}

auto Sorcery::ItemType::set_eff_inv(const Enums::Items::Effects::Invoke value)
	-> void {

	_invocation_effect = value;
}

auto Sorcery::ItemType::get_eff_def_str() const -> std::string {

	std::string effects{""};
	for (auto i = unenum(Enums::Items::Effects::Defensive::RESIST_COLD);
		 i <= unenum(Enums::Items::Effects::Defensive::PREVENT_DECAPITATION);
		 i++) {
		if (_defensive_effects[i]) {
			const auto eff_enum{
				magic_enum::enum_value<Enums::Items::Effects::Defensive>(i)};
			std::string str{
				magic_enum::enum_name<Enums::Items::Effects::Defensive>(
					eff_enum)};
			std::replace(str.begin(), str.end(), '_', ' ');
			effects.append(str);
			effects.append(", ");
		}
	}
	for (auto i =
			 unenum(Enums::Items::Effects::Defensive::PROTECTION_VS_ANIMAL);
		 i <= unenum(Enums::Items::Effects::Defensive::PROTECTION_VS_WERE);
		 i++) {
		if (_defensive_effects[i]) {
			const auto eff_enum{
				magic_enum::enum_value<Enums::Items::Effects::Defensive>(i)};
			std::string str{
				magic_enum::enum_name<Enums::Items::Effects::Defensive>(
					eff_enum)};
			std::replace(str.begin(), str.end(), '_', ' ');
			effects.append(str);
			effects.append(", ");
		}
	}

	return effects;
}

auto Sorcery::ItemType::get_eff_off_str() const -> std::string {

	std::string effects{""};
	for (auto i = unenum(Enums::Items::Effects::Offensive::PURPOSED_VS_ANIMAL);
		 i <= unenum(Enums::Items::Effects::Offensive::AUTOKILL); i++) {
		if (_offensive_effects[i]) {
			const auto eff_enum{
				magic_enum::enum_value<Enums::Items::Effects::Offensive>(i)};
			std::string str{
				magic_enum::enum_name<Enums::Items::Effects::Offensive>(
					eff_enum)};
			std::replace(str.begin(), str.end(), '_', ' ');
			effects.append(str);
			effects.append(", ");
		}
	}

	return effects;
}

auto Sorcery::ItemType::set_eff_inv_decay(const unsigned int value) -> void {

	_invocation_decay_chance = value;
}

auto Sorcery::ItemType::set_eff_use(const Enums::Magic::SpellID value) -> void {

	_use_effect = value;
}

auto Sorcery::ItemType::set_eff_use_decay(const unsigned int value) -> void {

	_use_decay_chance = value;
}

auto Sorcery::ItemType::set_decay_type_id(const Enums::Items::TypeID value)
	-> void {

	_decay_type = value;
}

auto Sorcery::ItemType::set_shop_inital_stock(const int value) -> void {

	_shop_initial_stock = value;
}

auto Sorcery::ItemType::set_discovered(const bool value) -> void {

	_discovered_by_player = value;
}

auto Sorcery::ItemType::set_desc(const std::string value) -> void {

	_description = value;
}

auto Sorcery::ItemType::set_gfx(const unsigned int value) -> void {

	_gfx = value;
}

auto Sorcery::ItemType::get_buy() const -> bool {

	return _buy;
}

auto Sorcery::ItemType::get_sell() const -> bool {

	return _sell;
}

auto Sorcery::ItemType::set_buy(const bool value) -> void {

	_buy = value;
}

auto Sorcery::ItemType::set_sell(const bool value) -> void {

	_sell = value;
}

auto Sorcery::ItemType::has_usable() const -> bool {

	return get_eff_use() != Enums::Magic::SpellID::NO_SPELL;
}

auto Sorcery::ItemType::has_invokable() const -> bool {

	return get_eff_inv() != Enums::Items::Effects::Invoke::NO_INV_EFFECT;
}

auto Sorcery::ItemType::get_equippable_display() const -> std::string {

	std::string equippable{"--------/---"};

	if (_usable[unenum(Enums::Character::Class::FIGHTER)])
		equippable[0] = 'F';
	if (_usable[unenum(Enums::Character::Class::MAGE)])
		equippable[1] = 'M';
	if (_usable[unenum(Enums::Character::Class::PRIEST)])
		equippable[2] = 'P';
	if (_usable[unenum(Enums::Character::Class::THIEF)])
		equippable[3] = 'T';
	if (_usable[unenum(Enums::Character::Class::BISHOP)])
		equippable[4] = 'B';
	if (_usable[unenum(Enums::Character::Class::SAMURAI)])
		equippable[5] = 'S';
	if (_usable[unenum(Enums::Character::Class::LORD)])
		equippable[6] = 'L';
	if (_usable[unenum(Enums::Character::Class::NINJA)])
		equippable[7] = 'N';

	if (_alignment[unenum(Enums::Character::Align::GOOD)])
		equippable[9] = 'g';
	if (_alignment[unenum(Enums::Character::Align::NEUTRAL)])
		equippable[10] = 'n';
	if (_alignment[unenum(Enums::Character::Align::EVIL)])
		equippable[11] = 'e';

	return equippable;
}

namespace Sorcery {

auto operator<<(std::ostream &out_stream, const Sorcery::ItemType &itemtype)
	-> std::ostream & {

	const auto type{unenum(itemtype.get_type_id())};
	const auto name{itemtype.get_known_name()};

	const auto text{std::format("{}: {}", type, name)};

	return out_stream << text << std::flush;
}
} // namespace Sorcery
