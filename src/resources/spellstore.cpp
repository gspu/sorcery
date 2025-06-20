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

#include "resources/spellstore.hpp"
#include "common/enum.hpp"
#include "common/include.hpp"
#include "common/macro.hpp"
#include "core/system.hpp"

Sorcery::SpellStore::SpellStore(System *system)
	: _system{system} {

	_load();
}

auto Sorcery::SpellStore::operator[](Enums::Magic::SpellID spell_id) const
	-> Spell {

	// Always assumes a spell can be found
	auto it{std::ranges::find_if(_spells.begin(), _spells.end(),
								 [&](const auto &spell) {
									 return (spell.id == spell_id);
								 })};
	return (*it);
}

auto Sorcery::SpellStore::operator()(Enums::Magic::SpellCategory category) const
	-> std::vector<Spell> {

	auto spells{_spells | std::views::filter([&](Spell spell) {
					return spell.category == category;
				})};

	std::vector<Spell> filtered;
	filtered.clear();
	for (auto spell : spells)
		filtered.emplace_back(spell);

	return filtered;
}

auto Sorcery::SpellStore::operator[](std::string name) const -> Spell {

	// Always assumes a spell can be found
	auto it{std::ranges::find_if(_spells.begin(), _spells.end(),
								 [&](const auto &spell) {
									 return (spell.name == name);
								 })};
	return (*it);
}

auto Sorcery::SpellStore::get_all() -> std::vector<Spell> {

	return _spells;
}

// Not sure why they are loaded here as opposed to a file
auto Sorcery::SpellStore::_load() -> void {

	_loaded = false;
	_spells.clear();

	// Mage Spells (grouped by level)

	// Level 1

	auto level{1};
	using enum Enums::Magic::SpellID;
	using enum Enums::Magic::SpellCategory;
	using enum Enums::Magic::SpellType;
	_spells.emplace_back(DUMAPIC, ARCANE, FIELD, level, false,
						 (*_system->strings)["SPELL_DUMAPIC_NAME"],
						 (*_system->strings)["SPELL_DUMAPIC_TITLE"],
						 (*_system->strings)["SPELL_DUMAPIC_DESC"]);
	_spells.emplace_back(HALITO, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_HALITO_NAME"],
						 (*_system->strings)["SPELL_HALITO_TITLE"],
						 (*_system->strings)["SPELL_HALITO_DESC"]);
	_spells.emplace_back(KATINO, ARCANE, DISABLE, level, false,
						 (*_system->strings)["SPELL_KATINO_NAME"],
						 (*_system->strings)["SPELL_KATINO_TITLE"],
						 (*_system->strings)["SPELL_KATINO_DESC"]);
	_spells.emplace_back(MOGREF, ARCANE, SUPPORT, level, false,
						 (*_system->strings)["SPELL_MOGREF_NAME"],
						 (*_system->strings)["SPELL_MOGREF_TITLE"],
						 (*_system->strings)["SPELL_MOGREF_DESC"]);

	// Level 2
	++level;
	_spells.emplace_back(DILTO, ARCANE, DISABLE, level, false,
						 (*_system->strings)["SPELL_DILTO_NAME"],
						 (*_system->strings)["SPELL_DILTO_TITLE"],
						 (*_system->strings)["SPELL_DILTO_DESC"]);
	_spells.emplace_back(SOPIC, ARCANE, SUPPORT, level, false,
						 (*_system->strings)["SPELL_SOPIC_NAME"],
						 (*_system->strings)["SPELL_SOPIC_TITLE"],
						 (*_system->strings)["SPELL_SOPIC_DESC"]);

	// Level 3
	++level;
	_spells.emplace_back(MAHALITO, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_MAHALITO_NAME"],
						 (*_system->strings)["SPELL_MAHALITO_TITLE"],
						 (*_system->strings)["SPELL_MAHALITO_DESC"]);
	_spells.emplace_back(MOLITO, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_MOLITO_NAME"],
						 (*_system->strings)["SPELL_MOLITO_TITLE"],
						 (*_system->strings)["SPELL_MOLITO_DESC"]);

	// Level 4
	++level;
	_spells.emplace_back(DALTO, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_DALTO_NAME"],
						 (*_system->strings)["SPELL_DALTO_TITLE"],
						 (*_system->strings)["SPELL_DALTO_DESC"]);
	_spells.emplace_back(LAHALITO, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_LAHALITO_NAME"],
						 (*_system->strings)["SPELL_LAHALITO_TITLE"],
						 (*_system->strings)["SPELL_LAHALITO_DESC"]);
	_spells.emplace_back(MORLIS, ARCANE, DISABLE, level, false,
						 (*_system->strings)["SPELL_MORLIS_NAME"],
						 (*_system->strings)["SPELL_MORLIS_TITLE"],
						 (*_system->strings)["SPELL_MORLIS_DESC"]);

	// Level 5
	++level;
	_spells.emplace_back(MADALTO, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_MADALTO_NAME"],
						 (*_system->strings)["SPELL_MADALTO_TITLE"],
						 (*_system->strings)["SPELL_MADALTO_DESC"]);
	_spells.emplace_back(MAKANITO, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_MAKANITO_NAME"],
						 (*_system->strings)["SPELL_MAKANITO_TITLE"],
						 (*_system->strings)["SPELL_MAKANITO_DESC"]);
	_spells.emplace_back(MAMORLIS, ARCANE, DISABLE, level, false,
						 (*_system->strings)["SPELL_MAMORLIS_NAME"],
						 (*_system->strings)["SPELL_MAMORLIS_TITLE"],
						 (*_system->strings)["SPELL_MAMORLIS_DESC"]);

	// Level 6
	++level;
	_spells.emplace_back(HAMAN, ARCANE, SUPPORT, level, false,
						 (*_system->strings)["SPELL_HAMAN_NAME"],
						 (*_system->strings)["SPELL_HAMAN_TITLE"],
						 (*_system->strings)["SPELL_HAMAN_DESC"]);
	_spells.emplace_back(LAKANITO, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_LAKANITO_NAME"],
						 (*_system->strings)["SPELL_LAKANITO_TITLE"],
						 (*_system->strings)["SPELL_LAKANITO_DESC"]);
	_spells.emplace_back(MASOPIC, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_MASOPIC_NAME"],
						 (*_system->strings)["SPELL_MASOPIC_TITLE"],
						 (*_system->strings)["SPELL_MASOPIC_DESC"]);
	_spells.emplace_back(ZILWAN, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_ZILWAN_NAME"],
						 (*_system->strings)["SPELL_ZILWAN_TITLE"],
						 (*_system->strings)["SPELL_ZILWAN_DESC"]);

	// Level 7
	++level;
	_spells.emplace_back(MAHAMAN, ARCANE, SUPPORT, level, false,
						 (*_system->strings)["SPELL_MAHAMAN_NAME"],
						 (*_system->strings)["SPELL_MAHAMAN_TITLE"],
						 (*_system->strings)["SPELL_MAHAMAN_DESC"]);
	_spells.emplace_back(MALOR, ARCANE, FIELD, level, false,
						 (*_system->strings)["SPELL_MALOR_NAME"],
						 (*_system->strings)["SPELL_MALOR_TITLE"],
						 (*_system->strings)["SPELL_MALOR_DESC"]);
	_spells.emplace_back(TILTOWAIT, ARCANE, ATTACK, level, false,
						 (*_system->strings)["SPELL_TILTOWAIT_NAME"],
						 (*_system->strings)["SPELL_TILTOWAIT_TITLE"],
						 (*_system->strings)["SPELL_TILTOWAIT_DESC"]);

	// Priest Spells (grouped by level)

	// Level 1
	level = 1u;
	_spells.emplace_back(BADIOS, DIVINE, ATTACK, level, false,
						 (*_system->strings)["SPELL_BADIOS_NAME"],
						 (*_system->strings)["SPELL_BADIOS_TITLE"],
						 (*_system->strings)["SPELL_BADIOS_DESC"]);
	_spells.emplace_back(DIOS, DIVINE, HEALING, level, false,
						 (*_system->strings)["SPELL_DIOS_NAME"],
						 (*_system->strings)["SPELL_DIOS_TITLE"],
						 (*_system->strings)["SPELL_DIOS_DESC"]);
	_spells.emplace_back(KALKI, DIVINE, SUPPORT, level, false,
						 (*_system->strings)["SPELL_KALKI_NAME"],
						 (*_system->strings)["SPELL_KALKI_TITLE"],
						 (*_system->strings)["SPELL_KALKI_DESC"]);
	_spells.emplace_back(MILWA, DIVINE, FIELD, level, false,
						 (*_system->strings)["SPELL_MILWA_NAME"],
						 (*_system->strings)["SPELL_MILWA_TITLE"],
						 (*_system->strings)["SPELL_MILWA_DESC"]);
	_spells.emplace_back(PORFIC, DIVINE, SUPPORT, level, false,
						 (*_system->strings)["SPELL_PORFIC_NAME"],
						 (*_system->strings)["SPELL_PORFIC_TITLE"],
						 (*_system->strings)["SPELL_PORFIC_DESC"]);

	// Level 2
	++level;
	_spells.emplace_back(CALFO, DIVINE, FIELD, level, false,
						 (*_system->strings)["SPELL_CALFO_NAME"],
						 (*_system->strings)["SPELL_CALFO_TITLE"],
						 (*_system->strings)["SPELL_CALFO_DESC"]);
	_spells.emplace_back(MANIFO, DIVINE, DISABLE, level, false,
						 (*_system->strings)["SPELL_MANIFO_NAME"],
						 (*_system->strings)["SPELL_MANIFO_TITLE"],
						 (*_system->strings)["SPELL_MANIFO_DESC"]);
	_spells.emplace_back(MATU, DIVINE, SUPPORT, level, false,
						 (*_system->strings)["SPELL_MATU_NAME"],
						 (*_system->strings)["SPELL_MATU_TITLE"],
						 (*_system->strings)["SPELL_MATU_DESC"]);
	_spells.emplace_back(MONTINO, DIVINE, DISABLE, level, false,
						 (*_system->strings)["SPELL_MONTINO_NAME"],
						 (*_system->strings)["SPELL_MONTINO_TITLE"],
						 (*_system->strings)["SPELL_MONTINO_DESC"]);

	// Level 3
	++level;
	_spells.emplace_back(BAMATU, DIVINE, SUPPORT, level, false,
						 (*_system->strings)["SPELL_BAMATU_NAME"],
						 (*_system->strings)["SPELL_BAMATU_TITLE"],
						 (*_system->strings)["SPELL_BAMATU_DESC"]);
	_spells.emplace_back(DIALKO, DIVINE, HEALING, level, false,
						 (*_system->strings)["SPELL_DIALKO_NAME"],
						 (*_system->strings)["SPELL_DIALKO_TITLE"],
						 (*_system->strings)["SPELL_DIALKO_DESC"]);
	_spells.emplace_back(LATUMAPIC, DIVINE, FIELD, level, false,
						 (*_system->strings)["SPELL_LATUMAPIC_NAME"],
						 (*_system->strings)["SPELL_LATUMAPIC_TITLE"],
						 (*_system->strings)["SPELL_LATUMAPIC_DESC"]);
	_spells.emplace_back(LOMILWA, DIVINE, FIELD, level, false,
						 (*_system->strings)["SPELL_LOMILWA_NAME"],
						 (*_system->strings)["SPELL_LOMILWA_TITLE"],
						 (*_system->strings)["SPELL_LOMILWA_DESC"]);

	// Level 4
	++level;
	_spells.emplace_back(BADIAL, DIVINE, ATTACK, level, false,
						 (*_system->strings)["SPELL_BADIAL_NAME"],
						 (*_system->strings)["SPELL_BADIAL_TITLE"],
						 (*_system->strings)["SPELL_BADIAL_DESC"]);
	_spells.emplace_back(DIAL, DIVINE, HEALING, level, false,
						 (*_system->strings)["SPELL_DIAL_NAME"],
						 (*_system->strings)["SPELL_DIAL_TITLE"],
						 (*_system->strings)["SPELL_DIAL_DESC"]);
	_spells.emplace_back(LATUMOFIS, DIVINE, HEALING, level, false,
						 (*_system->strings)["SPELL_LATUMOFIS_NAME"],
						 (*_system->strings)["SPELL_LATUMOFIS_TITLE"],
						 (*_system->strings)["SPELL_LATUMOFIS_DESC"]);
	_spells.emplace_back(MAPORFIC, DIVINE, FIELD, level, false,
						 (*_system->strings)["SPELL_MAPORFIC_NAME"],
						 (*_system->strings)["SPELL_MAPORFIC_TITLE"],
						 (*_system->strings)["SPELL_MAPORFIC_DESC"]);

	// Level 5
	++level;
	_spells.emplace_back(BADI, DIVINE, ATTACK, level, false,
						 (*_system->strings)["SPELL_BADI_NAME"],
						 (*_system->strings)["SPELL_BADI_TITLE"],
						 (*_system->strings)["SPELL_BADI_DESC"]);
	_spells.emplace_back(BADIALMA, DIVINE, ATTACK, level, false,
						 (*_system->strings)["SPELL_BADIALMA_NAME"],
						 (*_system->strings)["SPELL_BADIALMA_TITLE"],
						 (*_system->strings)["SPELL_BADIALMA_DESC"]);
	_spells.emplace_back(DI, DIVINE, HEALING, level, false,
						 (*_system->strings)["SPELL_DI_NAME"],
						 (*_system->strings)["SPELL_DI_TITLE"],
						 (*_system->strings)["SPELL_DI_DESC"]);
	_spells.emplace_back(DIALMA, DIVINE, HEALING, level, false,
						 (*_system->strings)["SPELL_DIALMA_NAME"],
						 (*_system->strings)["SPELL_DIALMA_TITLE"],
						 (*_system->strings)["SPELL_DIALMA_DESC"]);
	_spells.emplace_back(KANDI, DIVINE, FIELD, level, false,
						 (*_system->strings)["SPELL_KANDI_NAME"],
						 (*_system->strings)["SPELL_KANDI_TITLE"],
						 (*_system->strings)["SPELL_KANDI_DESC"]);
	_spells.emplace_back(LITOKAN, DIVINE, ATTACK, level, false,
						 (*_system->strings)["SPELL_LITOKAN_NAME"],
						 (*_system->strings)["SPELL_LITOKAN_TITLE"],
						 (*_system->strings)["SPELL_LITOKAN_DESC"]);

	// Level 6
	++level;
	_spells.emplace_back(LOKTOFEIT, DIVINE, FIELD, level, false,
						 (*_system->strings)["SPELL_LOKTOFEIT_NAME"],
						 (*_system->strings)["SPELL_LOKTOFEIT_TITLE"],
						 (*_system->strings)["SPELL_LOKTOFEIT_DESC"]);
	_spells.emplace_back(LORTO, DIVINE, ATTACK, level, false,
						 (*_system->strings)["SPELL_LORTO_NAME"],
						 (*_system->strings)["SPELL_LORTO_TITLE"],
						 (*_system->strings)["SPELL_LORTO_DESC"]);
	_spells.emplace_back(MABADI, DIVINE, ATTACK, level, false,
						 (*_system->strings)["SPELL_MABADI_NAME"],
						 (*_system->strings)["SPELL_MABADI_TITLE"],
						 (*_system->strings)["SPELL_MABADI_DESC"]);
	_spells.emplace_back(MADI, DIVINE, HEALING, level, false,
						 (*_system->strings)["SPELL_MADI_NAME"],
						 (*_system->strings)["SPELL_MADI_TITLE"],
						 (*_system->strings)["SPELL_MADI_DESC"]);

	// Level 7
	++level;
	_spells.emplace_back(KADORTO, DIVINE, HEALING, level, false,
						 (*_system->strings)["SPELL_KADORTO_NAME"],
						 (*_system->strings)["SPELL_KADORTO_TITLE"],
						 (*_system->strings)["SPELL_KADORTO_DESC"]);
	_spells.emplace_back(MALIKTO, DIVINE, ATTACK, level, false,
						 (*_system->strings)["SPELL_MALIKTO_NAME"],
						 (*_system->strings)["SPELL_MALIKTO_TITLE"],
						 (*_system->strings)["SPELL_MALIKTO_DESC"]);

	_loaded = true;
}