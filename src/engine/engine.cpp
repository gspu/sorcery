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

#include "engine/engine.hpp"
#include "common/macro.hpp"
#include "core/display.hpp"
#include "core/game.hpp"
#include "core/graphics.hpp"
#include "core/input.hpp"
#include "core/random.hpp"
#include "core/state.hpp"
#include "core/system.hpp"
#include "core/window.hpp"
#include "engine/define.hpp"
#include "engine/event.hpp"
#include "frontend/options.hpp"
#include "resources/componentstore.hpp"
#include "resources/factory.hpp"
#include "resources/levelstore.hpp"
#include "resources/stringstore.hpp"
#include "types/character.hpp"
#include "types/component.hpp"
#include "types/explore.hpp"
#include "types/level.hpp"
#include "types/tile.hpp"
#include "types/type.hpp"

// Standard Constructor
Sorcery::Engine::Engine(
	System *system, Display *display, Graphics *graphics, Game *game)
	: _system{system}, _display{display}, _graphics{graphics}, _game{game} {

	_game->hide_console();
	_cursor_coords = sf::Text{};

	// Setup the Factory
	_factory = std::make_unique<Factory>(_system, _display, _graphics, _game);
}

// Standard Destructor
Sorcery::Engine::~Engine() {
}

auto Sorcery::Engine::_generate_display() -> void {

	_reset_components();
	_initalise_components();
	_display->generate("engine_base_ui");
	_refresh();
	_place_components();
}

auto Sorcery::Engine::_initialise_state() -> void {

	_update_compass = false;
	_update_buffbar = false;
	_update_debuffbar = false;
	_update_icon_panels = false;
	_update_party_panel = false;
	_update_render = false;
	_update_search = false;
	_exit_maze_now = false;
	_pending_chute = false;
	_pending_elevator = false;
	_show_party_panel = true;
	_show_gui = true;
	_show_pool = false;
	_display_cursor = true;
	_pending_combat = false;
	_show_encounter = false;
	_next_combat = std::nullopt;
	_monochrome = true;
	_update_automap = true;

	_display->window->set_cursor_coord(true);
}

auto Sorcery::Engine::_reset_components() -> void {

	if (_camp_menu.get())
		_camp_menu.reset();
	if (_camp_menu_frame.get())
		_camp_menu_frame.reset();
	if (_search_menu.get())
		_search_menu.reset();
	if (_search_menu_frame.get())
		_search_menu_frame.reset();
	if (_view_frame_small.get())
		_view_frame_small.reset();
	if (_view_frame_big.get())
		_view_frame_big.reset();
	if (_get_menu.get())
		_get_menu.reset();
	if (_get_menu_frame.get())
		_get_menu_frame.reset();
	if (_action_menu.get())
		_action_menu.reset();
	if (_action_menu.get())
		_action_menu.reset();
	if (_elev_1_menu.get())
		_elev_1_menu.reset();
	if (_elev_1_menu_frame.get())
		_elev_1_menu_frame.reset();
	if (_elev_2_menu.get())
		_elev_2_menu.reset();
	if (_elev_2_menu_frame.get())
		_elev_2_menu_frame.reset();
	if (_confirm_exit.get())
		_confirm_exit.reset();
	if (_confirm_stairs.get())
		_confirm_stairs.reset();
	if (_confirm_search.get())
		_confirm_search.reset();
	if (_ouch.get())
		_ouch.reset();
	if (_encounter.get())
		_encounter.reset();
	if (_pit.get())
		_pit.reset();
	if (_chute.get())
		_chute.reset();
	if (_found.get())
		_found.reset();
	if (_elevator.get())
		_elevator.reset();
	if (_reorder.get())
		_reorder.reset();
	if (_inspect.get())
		_inspect.reset();
	if (_render.get())
		_render.reset();
	if (_graveyard.get())
		_graveyard.reset();
	if (_automap.get())
		_automap.reset();
	if (_compass.get())
		_compass.reset();
	if (_buffbar.get())
		_buffbar.reset();
	if (_debuffbar.get())
		_debuffbar.reset();
	if (_search.get())
		_search.reset();
	if (_left_icons.get())
		_left_icons.reset();
	if (_right_icons.get())
		_right_icons.reset();
	if (_map.get())
		_map.reset();
	if (_party_panel.get())
		_party_panel.reset();
	if (_character_display.get())
		_character_display.reset();
}

auto Sorcery::Engine::_initalise_components() -> void {

	// Get the Window and Graphics to Display
	_window = _display->window->get_window();

	// Setup Menus
	_camp_menu = _factory->make_menu("engine_base_ui:camp_menu", MTP::CAMP);
	_search_menu =
		_factory->make_menu("engine_base_ui:search_menu", MTP::SEARCH);
	_get_menu =
		_factory->make_menu("engine_base_ui:get_menu", MTP::CHARACTERS_HERE);
	_action_menu =
		_factory->make_menu("engine_base_ui:action_menu", MTP::ACTION);
	_elev_1_menu = _factory->make_menu(
		"engine_base_ui:elevator_a_d_menu", MTP::ELEVATOR_A_D);
	_elev_2_menu = _factory->make_menu(
		"engine_base_ui:elevator_a_f_menu", MTP::ELEVATOR_A_F);
	_camp_menu_frame =
		_factory->make_menu_frame("engine_base_ui:camp_menu_frame");
	_search_menu_frame =
		_factory->make_menu_frame("engine_base_ui:search_menu_frame");
	_get_menu_frame =
		_factory->make_menu_frame("engine_base_ui:get_menu_frame");
	_action_menu_frame =
		_factory->make_menu_frame("engine_base_ui:action_menu_frame");
	_elev_1_menu_frame =
		_factory->make_menu_frame("engine_base_ui:elevator_a_d_menu_frame");
	_elev_2_menu_frame =
		_factory->make_menu_frame("engine_base_ui:elevator_a_f_menu_frame");
	_view_frame_small = _factory->make_frame("engine_base_ui:view_frame_small");
	_view_frame_big = _factory->make_frame("engine_base_ui:view_frame_big");

	// Dialogs
	_ouch =
		_factory->make_dialog("engine_base_ui:ouch", WDT::TIMED, DELAY_OUCH);
	_pool = _factory->make_dialog("engine_base_ui:dialog_pool_gold_ok");
	_encounter = _factory->make_dialog(
		"engine_base_ui:an_encounter", WDT::TIMED, DELAY_ENCOUNTER);
	_pit = _factory->make_dialog("engine_base_ui:pit", WDT::TIMED, DELAY_PIT);
	_chute =
		_factory->make_dialog("engine_base_ui:chute", WDT::TIMED, DELAY_CHUTE);
	_found = _factory->make_dialog(
		"engine_base_ui:found_an_item", WDT::TIMED, DELAY_FIND_AN_ITEM);
	_elevator = _factory->make_dialog(
		"engine_base_ui:one_moment", WDT::TIMED, DELAY_ELEVATOR);
	_confirm_stairs =
		_factory->make_dialog("engine_base_ui:dialog_stairs", WDT::CONFIRM);
	_confirm_exit =
		_factory->make_dialog("engine_base_ui:dialog_exit", WDT::CONFIRM);
	_confirm_search =
		_factory->make_dialog("engine_base_ui:dialog_search", WDT::CONFIRM);
	_cur_char_frame = _factory->make_frame("engine_base_ui:character_frame");

	_character_display =
		std::make_unique<CharacterDisplay>(_system, _display, _graphics);

	// Modules
	_party_panel = std::make_unique<PartyPanel>(_system, _display, _graphics,
		_game, (*_display->layout)["engine_base_ui:party_panel_small"]);
	_reorder = std::make_unique<Reorder>(
		_system, _display, _graphics, _game, MMD::CAMP);
	_inspect = std::make_unique<Inspect>(
		_system, _display, _graphics, _game, MMD::CAMP);
	_render = std::make_unique<Render>(_system, _display, _graphics, _game);
	_graveyard =
		std::make_unique<Graveyard>(_system, _display, _graphics, _game);
	_automap = std::make_unique<AutoMap>(_system, _display, _graphics, _game,
		(*_display->layout)["global:automap"]);
	_compass = std::make_unique<Compass>(_system, _display, _graphics, _game,
		(*_display->layout)["global:compass"]);
	_buffbar = std::make_unique<BuffBar>(_system, _display, _graphics, _game,
		(*_display->layout)["global:buffbar"]);
	_debuffbar = std::make_unique<DebuffBar>(_system, _display, _graphics,
		_game, (*_display->layout)["global:debuffbar"]);
	_search = std::make_unique<Search>(_system, _display, _graphics, _game,
		(*_display->layout)["global:search"]);
	_console = std::make_unique<Console>(
		_display->window->get_gui(), _system, _display, _graphics, _game);
	_left_icons = std::make_unique<IconPanel>(_system, _display, _graphics,
		_game, (*_display->layout)["engine_base_ui:left_icon_panel"], true);
	_right_icons = std::make_unique<IconPanel>(_system, _display, _graphics,
		_game, (*_display->layout)["engine_base_ui:right_icon_panel"], false);
	_map = std::make_unique<Map>(_system, _display, _graphics, _game,
		(*_display->layout)["engine_base_ui:map"]);
}

auto Sorcery::Engine::_update_direction_indicator_timer() -> void {

	if (_show_dir) {
		if (!_dir_start)
			_dir_start = std::chrono::system_clock::now();

		_dir_time = std::chrono::system_clock::now();

		const auto elapsed{_dir_time.value() - _dir_start.value()};
		const auto elapsed_ms{
			std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)};
		if (elapsed_ms.count() > DELAY_DIRECTION) {
			_show_dir = false;
			_dir_start = std::nullopt;
		}
	}
}

auto Sorcery::Engine::_reset_direction_indicator() -> void {

	_show_dir = true;
	_dir_start = std::nullopt;
	_dir_time = std::nullopt;
}

auto Sorcery::Engine::_place_components() -> void {

	// Generate the Custom Components
	if (_show_gui)
		_party_panel->setPosition(
			(*_display->layout)["engine_base_ui:party_panel_small"].pos());
	else
		_party_panel->setPosition(_display->get_centre_x(_party_panel->width),
			(*_display->layout)["engine_base_ui:party_panel_big"].y);

	_automap->setPosition((*_display->layout)["global:automap"].pos());
	_compass->setPosition((*_display->layout)["global:compass"].pos());
	_buffbar->setPosition((*_display->layout)["global:buffbar"].pos());
	_debuffbar->setPosition((*_display->layout)["global:debuffbar"].pos());
	_left_icons->setPosition(
		(*_display->layout)["engine_base_ui:left_icon_panel"].pos());
	_right_icons->setPosition(
		(*_display->layout)["engine_base_ui:right_icon_panel"].pos());
	_map->setPosition((*_display->layout)["engine_base_ui:map"].pos());

	const Component search_c{(*_display->layout)["global:search"]};
	_search->setPosition(search_c.pos());
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
	_search_bounds =
		sf::IntRect{search_c.x, search_c.y, _search->width, _search->height};
#pragma GCC diagnostic pop
}

auto Sorcery::Engine::_refresh() const -> void {

	_render->refresh();
	_party_panel->refresh();
	_automap->refresh();
	_compass->refresh();
	_buffbar->refresh();
	_debuffbar->refresh();
	_search->refresh();
	_map->refresh();
	_left_icons->refresh(true);
	_right_icons->refresh(true);
}

auto Sorcery::Engine::_tile_explored(const Coordinate loc) const -> bool {

	return _game->state->explored[_game->state->get_depth()].at(loc);
}

auto Sorcery::Engine::_set_tile_explored(const Coordinate loc) -> void {

	_game->state->explored[_game->state->get_depth()].set(loc);
}

auto Sorcery::Engine::_set_maze_entry_start() -> void {

	_in_get = false;
	_in_map = false;
	_in_camp = true;
	_in_search = false;
	_in_action = false;
	_in_elev_1 = false;
	_in_elev_2 = false;
	_show_confirm_exit = false;
	_show_confirm_search = false;
	_show_ouch = false;
	_show_pit = false;
	_show_pool = false;
	_show_chute = false;
	_show_found = false;
	_show_elevator = false;
	_show_encounter = false;
	_show_party_panel = true;
	_show_gui = true;
	_exit_maze_now = false;
	_can_go_back = false;
	_automap->refresh();
	_map->refresh();
	_system->stop_pause();
	_last_movement = MAD::NO_DIRECTION;
	_can_run_event = false;

	const auto &start{_game->state->level->at(_game->state->get_player_pos())};

	if (!_tile_explored(_game->state->get_player_pos()))
		_set_tile_explored(_game->state->get_player_pos());

	// Now, we can also be on an elevator or a set of stairs too when we begin
	_show_confirm_stairs =
		(_game->state->get_player_pos() == Coordinate{0, 0}) &&
		(_game->state->get_depth() == -1);
	if ((_game->state->get_player_pos() == Coordinate{0, 0}) &&
		(_game->state->get_depth() == -1)) {

		_show_confirm_stairs = true;
		_game->state->set_player_facing(MAD::NORTH);
		_game->state->set_lit(false);
		if (!_tile_explored(_game->state->get_player_pos()))
			_set_tile_explored(_game->state->get_player_pos());
	} else
		_show_confirm_stairs = false;

	_display->set_input_mode(WIM::NAVIGATE_MENU);
	_opt_camp = _camp_menu->items.begin();
	_opt_act = _camp_menu->items.begin();
	_opt_search = _camp_menu->items.begin();

	auto has_elevator{start.has_elevator()};
	if (has_elevator) {

		// TODO: clunky need to fix this
		if (has_elevator.value().bottom_depth == -4) {
			_in_elev_1 = start.has(TLF::ELEVATOR);
			_opt_elev_1 = _elev_1_menu->items.end();
		} else if (has_elevator.value().bottom_depth == -9) {
			_in_elev_2 = start.has(TLF::ELEVATOR);
			_opt_elev_2 = _elev_2_menu->items.end();
		}
	}
}

auto Sorcery::Engine::_update_timers_and_components() -> void {

	_update_direction_indicator_timer();
	_ouch->update();
	_encounter->update();
	_pit->update();
	_chute->update();
	_found->update();
	_elevator->update();
}

auto Sorcery::Engine::_check_for_pending_events() -> void {

	if (auto pending{_system->update_pause()}; pending) {
		if (_pending_chute) {

			const auto tile{
				_game->state->level->at(_game->state->get_player_pos())};
			if (tile.has(TLF::CHUTE)) {

				const auto dest{tile.has_teleport().value()};
				const auto dest_level{dest.to_level};
				Level level{((*_game->levelstore)[dest_level]).value()};
				_game->state->set_current_level(&level);
				_game->state->set_player_pos(dest.to_loc);
				_game->state->set_player_prev_depth(_game->state->get_depth());
				_game->state->set_depth(dest_level);

				_set_tile_explored(_game->state->get_player_pos());
				_set_refresh_ui();
				_pending_chute = false;
				_can_go_back = true;
				_display->set_disc(true);
				_refresh_display();
				_game->save_game();
				_display->set_disc(false);
			}
		} else if (_pending_elevator) {

			const auto tile{
				_game->state->level->at(_game->state->get_player_pos())};
			if (tile.has(TLF::ELEVATOR)) {

				Level level{((*_game->levelstore)[_dest_floor]).value()};
				_game->state->set_current_level(&level);
				_game->state->set_player_prev_depth(_game->state->get_depth());
				_game->state->set_depth(_dest_floor);
				_set_tile_explored(_game->state->get_player_pos());
				_set_refresh_ui();

				_pending_elevator = false;
				_can_go_back = true;
				_display->set_disc(true);
				_refresh_display();
				_game->save_game();
				_display->set_disc(false);
				_dest_floor = 0;
			}
		} else if (_pending_combat) {

			// TODO: combat!!!

			// do combat here

			_set_refresh_ui();

			_pending_combat = false;
		}
	}
}

auto Sorcery::Engine::_do_pause(sf::Event &event) -> void {

	_refresh_display();

	if (auto any_event{_window->pollEvent(event)}; any_event) {
		if (_system->input->check(CIN::ANYTHING, event))
			_system->stop_pause();
	}
}

auto Sorcery::Engine::_unhightlight_panels() -> void {

	if (_left_icons->selected)
		_left_icons->selected = std::nullopt;
	if (_right_icons->selected)
		_right_icons->selected = std::nullopt;
	if (_party_panel->selected)
		_party_panel->selected = std::nullopt;
}

auto Sorcery::Engine::_handle_confirm_search(const sf::Event &event) -> bool {

	_unhightlight_panels();

	if (auto input{_confirm_search->handle_input(event)}; input) {
		if ((input.value() == WDB::CLOSE) || (input.value() == WDB::NO)) {
			_display->set_input_mode(WIM::IN_GAME);
			_show_confirm_search = false;
			return false;
		} else if (input.value() == WDB::YES) {
			_display->set_input_mode(WIM::IN_GAME);
			_show_confirm_search = false;

			// now if we have an item, or if it is something like combat
			const auto at_loc{_game->state->get_player_pos()};
			if (_game->state->level->at(at_loc).has_event()) {

				// use the quest item flags for now
				switch (auto event{
					_game->state->level->at(at_loc).has_event().value()};
					event) {
				case MAV::SILVER_KEY: {
					_show_found = true;

					// random character who has inventory free unless its a
					// targeted search (TODO)
					const auto &character{_game->characters[_game->state
							->get_character_by_position(1)
							.value()]};
					const auto text{fmt::format("{}{}", character.get_name(),
						(*_display->string)["FOUND_AN_ITEM"])};
					_found->set(
						(*_display->layout)["engine_base_ui:found_an_item"],
						text);
					_found->reset_timed();

					if (!_game->state
							->quest_item_flags[unenum(ItemQuest::SILVER_KEY)])
						_game->state
							->quest_item_flags[unenum(ItemQuest::SILVER_KEY)] =
							true;

					return true;
				} break;

				case MAV::BRONZE_KEY: {
					_show_found = true;

					// random character who has inventory free unless its a
					// targeted search (TODO)
					const auto &character{_game->characters[_game->state
							->get_character_by_position(1)
							.value()]};
					const auto text{fmt::format("{}{}", character.get_name(),
						(*_display->string)["FOUND_AN_ITEM"])};
					_found->set(
						(*_display->layout)["engine_base_ui:found_an_item"],
						text);
					_found->reset_timed();

					if (!_game->state
							->quest_item_flags[unenum(ItemQuest::BRONZE_KEY)])
						_game->state
							->quest_item_flags[unenum(ItemQuest::BRONZE_KEY)] =
							true;

					return true;
				} break;

				case MAV::GOLD_KEY: {
					_show_found = true;

					// random character who has inventory free unless its a
					// targeted search (TODO)
					const auto &character{_game->characters[_game->state
							->get_character_by_position(1)
							.value()]};
					const auto text{fmt::format("{}{}", character.get_name(),
						(*_display->string)["FOUND_AN_ITEM"])};
					_found->set(
						(*_display->layout)["engine_base_ui:found_an_item"],
						text);
					_found->reset_timed();

					if (!_game->state
							->quest_item_flags[unenum(ItemQuest::GOLD_KEY)])
						_game->state
							->quest_item_flags[unenum(ItemQuest::GOLD_KEY)] =
							true;

					return true;
				} break;

				case MAV::BEAR_STATUE: {
					_show_found = true;

					// random character who has inventory free unless its a
					// targeted search (TODO)
					const auto &character{_game->characters[_game->state
							->get_character_by_position(1)
							.value()]};
					const auto text{fmt::format("{}{}", character.get_name(),
						(*_display->string)["FOUND_AN_ITEM"])};
					_found->set(
						(*_display->layout)["engine_base_ui:found_an_item"],
						text);
					_found->reset_timed();

					if (!_game->state
							->quest_item_flags[unenum(ItemQuest::BEAR_STATUE)])
						_game->state
							->quest_item_flags[unenum(ItemQuest::BEAR_STATUE)] =
							true;

					return true;
				} break;

				case MAV::FROG_STATUE: {
					_show_found = true;

					// random character who has inventory free unless its a
					// targeted search (TODO)
					const auto &character{_game->characters[_game->state
							->get_character_by_position(1)
							.value()]};
					const auto text{fmt::format("{}{}", character.get_name(),
						(*_display->string)["FOUND_AN_ITEM"])};
					_found->set(
						(*_display->layout)["engine_base_ui:found_an_item"],
						text);
					_found->reset_timed();

					if (!_game->state
							->quest_item_flags[unenum(ItemQuest::FROG_STATUE)])
						_game->state
							->quest_item_flags[unenum(ItemQuest::FROG_STATUE)] =
							true;

					return true;
				} break;
				case MAV::MURPHYS_GHOSTS: {

					_show_encounter = true;
					_next_combat = CombatType::MURPHYS_GHOSTS;
					return true;
				} break;

				default:
					break;
				}
			}
		}
	}

	return false;
}

auto Sorcery::Engine::_handle_confirm_exit(const sf::Event &event) -> void {

	_unhightlight_panels();

	auto input{_confirm_exit->handle_input(event)};
	if (input) {
		if ((input.value() == WDB::CLOSE) || (input.value() == WDB::NO)) {
			_display->set_input_mode(WIM::NAVIGATE_MENU);
			_show_confirm_exit = false;
		} else if (input.value() == WDB::YES) {
			_window->close();
		}
	}
}

auto Sorcery::Engine::_handle_in_search(const sf::Event &event)
	-> std::optional<int> {

	_unhightlight_panels();

	if (_system->input->check(CIN::CANCEL, event))
		_in_search = false;

	if (_system->input->check(CIN::BACK, event))
		_in_search = false;

	if (_system->input->check(CIN::SHOW_HIDE_CONSOLE, event))
		_game->toggle_console();
	else if (_system->input->check(CIN::UP, event))
		_opt_search = _search_menu->choose_previous();
	else if (_system->input->check(CIN::DOWN, event))
		_opt_search = _search_menu->choose_next();
	else if (_system->input->check(CIN::MOVE, event))
		_opt_search = _search_menu->set_mouse_selected(_display->get_cur());
	else if (_system->input->check(CIN::CONFIRM, event)) {

		// We have selected something from the menu
		if (_opt_search) {

			if (const MIM opt{(*_opt_search.value()).item};
				opt == MIM::AC_LEAVE) {

				_display->set_disc(true);
				_refresh_display();
				_game->save_game();
				_display->set_disc(false);

				_party_panel->refresh();
				_in_search = false;
				_display->generate("engine_base_ui");
				_display->set_input_mode(WIM::IN_GAME);
				return CONTINUE;
			} else if (opt == MIM::AC_SEARCH_CHARACTERS) {
				_in_search = false;
				_in_get = true;
				_get_menu->reload();
				_display->generate("engine_base_ui");
				_display->set_input_mode(WIM::IN_GAME);
				return CONTINUE;
			}
		}
	}

	return std::nullopt;
}

auto Sorcery::Engine::_handle_in_action(const sf::Event &event)
	-> std::optional<int> {

	_unhightlight_panels();

	if (_system->input->check(CIN::CANCEL, event))
		_in_action = false;

	if (_system->input->check(CIN::BACK, event))
		_in_action = false;

	if (_system->input->check(CIN::SHOW_HIDE_CONSOLE, event))
		_game->toggle_console();
	else if (_system->input->check(CIN::UP, event))
		_opt_act = _action_menu->choose_previous();
	else if (_system->input->check(CIN::DOWN, event))
		_opt_act = _action_menu->choose_next();
	else if (_system->input->check(CIN::MOVE, event))
		_opt_act = _action_menu->set_mouse_selected(_display->get_cur());
	else if (_system->input->check(CIN::CONFIRM, event)) {

		// We have selected something from the menu
		if (_opt_act) {

			if (const MIM opt{(*_opt_act.value()).item}; opt == MIM::AC_LEAVE) {

				_display->set_disc(true);
				_refresh_display();
				_game->save_game();
				_display->set_disc(false);

				_party_panel->refresh();
				_in_action = false;
				_display->generate("engine_base_ui");
				_display->set_input_mode(WIM::IN_GAME);
				return CONTINUE;
			} else if (opt == MIM::AC_SEARCH_CHARACTERS) {

				_in_action = false;
				_in_get = true;
				_get_menu->reload();
				return CONTINUE;
			}
		}
	}

	return std::nullopt;
}

auto Sorcery::Engine::_handle_in_get(const sf::Event &event)
	-> std::optional<int> {

	_unhightlight_panels();

	if (_system->input->check(CIN::CANCEL, event))
		_in_get = false;

	if (_system->input->check(CIN::BACK, event))
		_in_get = false;

	if (_system->input->check(CIN::SHOW_HIDE_CONSOLE, event))
		_game->toggle_console();
	else if (_system->input->check(CIN::UP, event))
		_opt_get = _get_menu->choose_previous();
	else if (_system->input->check(CIN::DOWN, event))
		_opt_get = _get_menu->choose_next();
	else if (_system->input->check(CIN::MOVE, event))
		_opt_get = _get_menu->set_mouse_selected(_display->get_cur());
	else if (_system->input->check(CIN::CONFIRM, event)) {

		// We have selected something from the menu
		if (_opt_get) {

			if (const MIM opt{(*_opt_get.value()).item}; opt == MIM::AC_LEAVE) {

				_display->set_disc(true);
				_refresh_display();
				_game->save_game();
				_display->set_disc(false);

				_party_panel->refresh();
				_in_get = false;
				_display->generate("engine_base_ui");
				_display->set_input_mode(WIM::IN_GAME);
				return CONTINUE;
			} else if ((*_opt_get.value()).type == MIT::ENTRY) {

				const auto chosen{(*_opt_get.value()).index};
				_cur_char = &_game->characters[chosen];
				if (_cur_char) {
					_cur_char.value()->set_location(CHL::PARTY);
					_game->state->add_character_by_id(chosen);

					_display->set_disc(true);
					_refresh_display();
					_game->save_game();
					_display->set_disc(false);
				}

				_party_panel->refresh();
				_get_menu->reload();
				_opt_get = _get_menu->items.begin();
			}
		}
	}

	return std::nullopt;
}

auto Sorcery::Engine::_handle_in_camp(const sf::Event &event)
	-> std::optional<int> {

	_unhightlight_panels();

	if (_system->input->check(CIN::CANCEL, event))
		_in_camp = false;

	if (_system->input->check(CIN::BACK, event))
		_in_camp = false;

	if (_system->input->check(CIN::SHOW_HIDE_CONSOLE, event))
		_game->toggle_console();
	else if (_system->input->check(CIN::UP, event))
		_opt_camp = _camp_menu->choose_previous();
	else if (_system->input->check(CIN::DOWN, event))
		_opt_camp = _camp_menu->choose_next();
	else if (_system->input->check(CIN::MOVE, event))
		_opt_camp = _camp_menu->set_mouse_selected(_display->get_cur());
	else if (_system->input->check(CIN::CONFIRM, event)) {

		// We have selected something from the menu
		if (_opt_camp) {

			if (const MIM opt{(*_opt_camp.value()).item};
				opt == MIM::CP_LEAVE) {

				_display->set_disc(true);
				_refresh_display();
				_game->save_game();
				_display->set_disc(false);

				_party_panel->refresh();
				_in_camp = false;
				_display->generate("engine_base_ui");
				_display->set_input_mode(WIM::IN_GAME);
				return CONTINUE;
			} else if (opt == MIM::CP_SAVE) {

				auto party{_game->state->get_party_characters()};
				for (auto &[id, character] : _game->characters) {
					if (std::find(party.begin(), party.end(), id) !=
						party.end()) {
						character.set_location(CHL::MAZE);
					}
				}

				_display->set_disc(true);
				_refresh_display();
				_game->save_game();
				_display->set_disc(false);
				_game->state->clear_party();
				return STOP_ENGINE;
			} else if (opt == MIM::ITEM_QUIT) {
				_show_confirm_exit = true;
				return CONTINUE;
			} else if (opt == MIM::CP_OPTIONS) {

				auto options{
					std::make_unique<Options>(_system, _display, _graphics)};
				if (auto result{options->start()}; result == STOP_ALL) {
					options->stop();
					return STOP_ALL;
				}
				options->stop();
				_party_panel->refresh();
				_display->generate("engine_base_ui");
			} else if (opt == MIM::CP_INSPECT) {
				_party_panel->refresh();
				if (auto result{_inspect->start(std::nullopt)};
					result == MIM::ITEM_ABORT) {
					_inspect->stop();
					return STOP_ALL;
				}
				_inspect->stop();
				_party_panel->refresh();
				_display->generate("engine_base_ui");
			} else if (opt == MIM::CP_REORDER) {
				_party_panel->refresh();
				if (auto new_party{_reorder->start()}; new_party) {

					// TODO: handle aborts here too
					_game->state->set_party(new_party.value());

					_display->set_disc(true);
					_refresh_display();
					_game->save_game();
					_display->set_disc(false);

					_party_panel->refresh();
				}
				_reorder->stop();
				_party_panel->refresh();
				_display->generate("engine_base_ui");
			}
		}
	}

	return std::nullopt;
}

// Will return true in the event of a wipe
auto Sorcery::Engine::_check_for_wipe() const -> bool {

	const auto party{_game->state->get_party_characters()};
	for (auto &[id, character] : _game->characters) {
		if (std::find(party.begin(), party.end(), id) != party.end()) {
			if ((character.get_status() == CHT::OK) ||
				(character.get_status() == CHT::AFRAID) ||
				(character.get_status() == CHT::SILENCED))
				return false;
		}
	}

	return true;
}

auto Sorcery::Engine::_do_wipe() -> int {

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	const auto party{_game->state->get_party_characters()};
	for (auto &[id, character] : _game->characters) {
		if (std::find(party.begin(), party.end(), id) != party.end()) {
			character.set_location(CHL::MAZE);
			character.set_current_hp(0);
		}
	}
	_graveyard->start();
	_graveyard->stop();

	_display->set_disc(true);
	_refresh_display();
	_game->save_game();
	_display->set_disc(false);

	_game->state->clear_party();

	return STOP_ENGINE;
}

auto Sorcery::Engine::_handle_elevator_a_f(const sf::Event &event)
	-> std::optional<int> {

	_unhightlight_panels();

	if (_system->input->check(CIN::CANCEL, event))
		_in_elev_2 = false;

	if (_system->input->check(CIN::BACK, event))
		_in_elev_2 = false;

	if (_system->input->check(CIN::SHOW_HIDE_CONSOLE, event))
		_game->toggle_console();
	else if (_system->input->check(CIN::UP, event))
		_opt_elev_2 = _elev_2_menu->choose_previous();
	else if (_system->input->check(CIN::DOWN, event))
		_opt_elev_2 = _elev_2_menu->choose_next();
	else if (_system->input->check(CIN::MOVE, event))
		_opt_elev_2 = _elev_2_menu->set_mouse_selected(_display->get_cur());
	else if (_system->input->check(CIN::CONFIRM, event)) {

		// We have selected something from the menu
		if (_opt_elev_2) {

			if (const MIM opt{(*_opt_elev_2.value()).item};
				opt == MIM::EL_LEAVE) {
				_in_elev_2 = false;
				_display->generate("engine_base_ui");
				_display->set_input_mode(WIM::IN_GAME);
				_pending_elevator = false;
				_dest_floor = 0;
				return CONTINUE;
			} else if ((opt == MIM::EL_A) &&
					   (_game->state->get_depth() != -4)) {
				_dest_floor = -4;
				_pending_elevator = true;
				_elevator_if();
			} else if ((opt == MIM::EL_B) &&
					   (_game->state->get_depth() != -5)) {
				_dest_floor = -5;
				_pending_elevator = true;
				_elevator_if();
			} else if ((opt == MIM::EL_C) &&
					   (_game->state->get_depth() != -6)) {
				_dest_floor = -6;
				_pending_elevator = true;
				_elevator_if();
			} else if ((opt == MIM::EL_D) &&
					   (_game->state->get_depth() != -7)) {
				_dest_floor = -7;
				_pending_elevator = true;
				_elevator_if();
			} else if ((opt == MIM::EL_E) &&
					   (_game->state->get_depth() != -8)) {
				_dest_floor = -8;
				_pending_elevator = true;
				_elevator_if();
			} else if ((opt == MIM::EL_F) &&
					   (_game->state->get_depth() != -9)) {
				_dest_floor = -9;
				_pending_elevator = true;
				_elevator_if();
			}
		} else {
			_in_elev_2 = false;
			_display->generate("engine_base_ui");
			_display->set_input_mode(WIM::IN_GAME);
			_pending_elevator = false;
			_dest_floor = 0;
		}
	}

	return std::nullopt;
}

auto Sorcery::Engine::_handle_elevator_a_d(const sf::Event &event)
	-> std::optional<int> {

	_unhightlight_panels();

	if (_system->input->check(CIN::CANCEL, event))
		_in_elev_1 = false;

	if (_system->input->check(CIN::BACK, event))
		_in_elev_1 = false;

	if (_system->input->check(CIN::SHOW_HIDE_CONSOLE, event))
		_game->toggle_console();
	else if (_system->input->check(CIN::UP, event))
		_opt_elev_1 = _elev_1_menu->choose_previous();
	else if (_system->input->check(CIN::DOWN, event))
		_opt_elev_1 = _elev_1_menu->choose_next();
	else if (_system->input->check(CIN::MOVE, event))
		_opt_elev_1 = _elev_1_menu->set_mouse_selected(_display->get_cur());
	else if (_system->input->check(CIN::CONFIRM, event)) {

		// We have selected something from the menu
		if (_opt_elev_1) {

			if (const MIM opt{(*_opt_elev_1.value()).item};
				opt == MIM::EL_LEAVE) {
				_in_elev_1 = false;
				_display->generate("engine_base_ui");
				_display->set_input_mode(WIM::IN_GAME);
				_pending_elevator = false;
				_dest_floor = 0;
				return CONTINUE;
			} else if ((opt == MIM::EL_A) &&
					   (_game->state->get_depth() != -1)) {
				_dest_floor = -1;
				_pending_elevator = true;
				_elevator_if();
			} else if ((opt == MIM::EL_B) &&
					   (_game->state->get_depth() != -2)) {
				_dest_floor = -2;
				_pending_elevator = true;
				_elevator_if();
			} else if ((opt == MIM::EL_C) &&
					   (_game->state->get_depth() != -3)) {
				_dest_floor = -3;
				_pending_elevator = true;
				_elevator_if();
			} else if ((opt == MIM::EL_D) &&
					   (_game->state->get_depth() != -4)) {
				_dest_floor = -4;
				_pending_elevator = true;
				_elevator_if();
			} else {
				_in_elev_1 = false;
				_display->generate("engine_base_ui");
				_display->set_input_mode(WIM::IN_GAME);
				_pending_elevator = false;
				_dest_floor = 0;
			}
		}
	}

	return std::nullopt;
}

auto Sorcery::Engine::_handle_in_map(const sf::Event &event)
	-> std::optional<int> {

	if ((_system->input->check(CIN::MAZE_SHOW_MAP, event)) ||
		(_system->input->check(CIN::CANCEL, event)) ||
		(_system->input->check(CIN::CONFIRM, event))) {

		_set_refresh_ui();

		return CONTINUE;
	}

	return std::nullopt;
}

auto Sorcery::Engine::_unpoison_characters() -> void {

	const auto party{_game->state->get_party_characters()};
	for (auto &[id, character] : _game->characters) {
		if (std::find(party.begin(), party.end(), id) != party.end()) {
			if ((character.get_status() == CHT::AFRAID) ||
				(character.get_status() == CHT::SILENCED))
				character.set_status(CHT::OK);
			character.set_poisoned_rate(0);
		}
	}
}

auto Sorcery::Engine::_triage_characters() -> void {

	const auto party{_game->state->get_party_characters()};
	for (auto &[id, character] : _game->characters) {
		if (std::find(party.begin(), party.end(), id) != party.end()) {
			if ((character.get_status() == CHT::DEAD) ||
				(character.get_status() == CHT::ASHES) ||
				(character.get_status() == CHT::STONED) ||
				(character.get_status() == CHT::HELD)) {
				character.set_location(CHL::TEMPLE);
				_game->state->remove_character_by_id(id);
			} else if (character.get_status() == CHT::LOST) {
				character.set_location(CHL::TRAINING);
				_game->state->remove_character_by_id(id);
			}
		}
	}
}

auto Sorcery::Engine::_handle_in_game(const sf::Event &event)
	-> std::optional<int> {

	// Handle any events or encounters first - will run once then set
	// _can_run_event to false
	_event_if();
	_encounter_if();
	_combat_if();
	if (_exit_maze_now) {

		_display->set_disc(true);
		_refresh_display();
		_game->save_game();
		_display->set_disc(false);

		_exit_maze_now = false;
		return STOP_ENGINE;
	}

	// Various Debug Commands can be put here
	if ((event.type == sf::Event::KeyPressed) &&
		(event.key.code == sf::Keyboard::F2))
		_debug_go_to_graveyard();
	else if ((event.type == sf::Event::KeyPressed) &&
			 (event.key.code == sf::Keyboard::F3))
		_debug_go_back();
	else if ((event.type == sf::Event::KeyPressed) &&
			 (event.key.code == sf::Keyboard::F4))
		_debug_send_non_party_characters_to_tavern();
	else if ((event.type == sf::Event::KeyPressed) &&
			 (event.key.code == sf::Keyboard::F5))
		_debug_kill_non_party_characters();
	else if ((event.type == sf::Event::KeyPressed) &&
			 (event.key.code == sf::Keyboard::F6))
		_debug_heal_party_to_full();
	else if ((event.type == sf::Event::KeyPressed) &&
			 (event.key.code == sf::Keyboard::F7))
		_debug_set_quest_item_flags();
	else if ((event.type == sf::Event::KeyPressed) &&
			 (event.key.code == sf::Keyboard::F8))
		_debug_clear_quest_item_flags();
	else if ((event.type == sf::Event::KeyPressed) &&
			 (event.key.code == sf::Keyboard::F9)) {
		_monochrome = true;
		_debug_monochrome_wireframe();
	} else if ((event.type == sf::Event::KeyPressed) &&
			   (event.key.code == sf::Keyboard::F10)) {
		_monochrome = false;
		_debug_colour_wireframe();
	} else if ((event.type == sf::Event::KeyPressed) &&
			   (event.key.code == sf::Keyboard::Pause)) {
		_display_cursor = true;
		_refresh_display();
	} else if ((event.type == sf::Event::KeyPressed) &&
			   (event.key.code == sf::Keyboard::F11))
		_debug_light_on();
	else if ((event.type == sf::Event::KeyPressed) &&
			 (event.key.code == sf::Keyboard::F12))
		_debug_light_off();

	if (_system->input->check(CIN::MAZE_SHOW_MAP, event)) {
		_in_map = !_in_map;
		_set_refresh_ui();
	} else if (_system->input->check(CIN::MAZE_SEARCH, event)) {
		_in_search = true;
		_set_refresh_ui();
	} else if (_system->input->check(CIN::MAZE_INSPECT, event)) {
		_in_get = true;
		_get_menu->reload();
		_set_refresh_ui();
	} else if (_system->input->check(CIN::MAZE_CAMP, event)) {
		_in_camp = true;
		_set_refresh_ui();
	} else if (_system->input->check(CIN::MAZE_STATUSBAR_TOGGLE, event)) {
		_show_party_panel = !_show_party_panel;
		_set_refresh_ui();
	} else if (_system->input->check(CIN::MAZE_GUI_TOGGLE, event)) {
		_show_gui = !_show_gui;
		_place_components();
		_set_refresh_ui();
	}

	if (_show_ouch) {
		_show_dir = false;
		if (const auto input{_ouch->handle_input(event)}; input) {
			if (input.value() == WDB::OK) {
				_display->set_input_mode(WIM::IN_GAME);
				_show_ouch = false;
				_ouch->set_valid(false);
			}
		}
	} else if (_show_encounter) {
		_show_dir = false;
		if (auto input{_encounter->handle_input(event)}; input) {
			if (input.value() == WDB::OK) {
				_display->set_input_mode(WIM::IN_GAME);
				_show_encounter = false;
				_encounter->set_valid(false);
			}
		}
	} else if (_show_found) {
		_show_dir = false;
		if (const auto input{_found->handle_input(event)}; input) {
			if (input.value() == WDB::OK) {
				_display->set_input_mode(WIM::IN_GAME);
				_show_found = false;
				_found->set_valid(false);
			}
		}
	} else if (_show_pit) {
		if (const auto input{_pit->handle_input(event)}; input) {
			if (input.value() == WDB::OK) {
				_display->set_input_mode(WIM::IN_GAME);
				_show_pit = false;
				_pit->set_valid(false);
			}
		}
	} else if (_show_chute) {
		if (const auto input{_chute->handle_input(event)}; input) {
			if (input.value() == WDB::OK) {
				_display->set_input_mode(WIM::IN_GAME);
				_show_chute = false;
				_chute->set_valid(false);
				if (const auto &next_tile{_game->state->level->at(
						_game->state->get_player_pos())};
					(next_tile.is(TLP::DARKNESS)) && (_game->state->get_lit()))
					_game->state->set_lit(false);
			}
		}
	} else if (_show_elevator) {
		if (const auto input{_elevator->handle_input(event)}; input) {
			if (input.value() == WDB::OK) {
				_display->set_input_mode(WIM::IN_GAME);
				_show_elevator = false;
				_elevator->set_valid(false);
				if (const auto &next_tile{_game->state->level->at(
						_game->state->get_player_pos())};
					(next_tile.is(TLP::DARKNESS)) && (_game->state->get_lit()))
					_game->state->set_lit(false);
			}
		}
	} else if (_show_confirm_stairs) {

		_unhightlight_panels();

		if (auto input{_confirm_stairs->handle_input(event)}; input) {
			if ((input.value() == WDB::CLOSE) || (input.value() == WDB::NO)) {
				_display->set_input_mode(WIM::IN_GAME);
				_show_confirm_stairs = false;
			} else if (input.value() == WDB::YES) {
				_show_confirm_stairs = false;

				if (const auto at_loc{_game->state->get_player_pos()};
					(at_loc == Coordinate{0, 0}) &&
					(_game->state->get_depth() == -1)) {

					// On return to time - TODO: need also to do this on MALOR
					// back as well
					_unpoison_characters();
					_triage_characters();

					_display->set_disc(true);
					_refresh_display();
					_game->save_game();
					_display->set_disc(false);

					return STOP_ENGINE;
				} else {

					_stairs_if();
					if (!_tile_explored(_game->state->get_player_pos()))
						_set_tile_explored(_game->state->get_player_pos());
					_update_automap = true;
					_show_confirm_stairs = true;

					_display->set_disc(true);
					_refresh_display();
					_game->save_game();
					_display->set_disc(false);
				}
			}
		}
	} else {
		if (_system->input->check(CIN::MAZE_TURN_AROUND, event)) {
			_show_dir = true;
			_reset_direction_indicator();
			_turn_around();
			_spinner_if();
			_set_refresh_ui();
			_game->pass_turn();
		}
		if ((_system->input->check(CIN::LEFT, event)) ||
			(_system->input->check(CIN::MAZE_LEFT, event))) {
			_show_dir = true;
			_reset_direction_indicator();
			_turn_left();
			_spinner_if();
			_set_refresh_ui();
			_game->pass_turn();
		} else if ((_system->input->check(CIN::RIGHT, event)) ||
				   (_system->input->check(CIN::MAZE_RIGHT, event))) {
			_show_dir = true;
			_reset_direction_indicator();
			_turn_right();
			_spinner_if();
			_set_refresh_ui();
			_game->pass_turn();
		} else if ((_system->input->check(CIN::UP, event)) ||
				   (_system->input->check(CIN::MAZE_FORWARD, event))) {
			_game->pass_turn();
			if (auto has_moved{_move_forward()}; !has_moved) {
				_show_dir = false;
				_show_ouch = true;
				_ouch->reset_timed();
			} else {
				_show_dir = true;
				if (!_tile_explored(_game->state->get_player_pos()))
					_set_tile_explored(_game->state->get_player_pos());
				_reset_direction_indicator();
				_teleport_if();
				_spinner_if();
				_pit_if();
				_chute_if();
				_update_automap = true;
			}
			if (_exit_maze_now) {

				_display->set_disc(true);
				_refresh_display();
				_game->save_game();
				_display->set_disc(false);

				_exit_maze_now = false;
				return STOP_ENGINE;
			}
			_set_refresh_ui();
			_can_run_event = true;
		} else if ((_system->input->check(CIN::DOWN, event)) ||
				   (_system->input->check(CIN::MAZE_BACKWARD, event))) {
			_game->pass_turn();
			if (auto has_moved{_move_backward()}; !has_moved) {
				_show_dir = false;
				_show_ouch = true;
				_ouch->reset_timed();
			} else {
				_show_dir = true;
				if (!_tile_explored(_game->state->get_player_pos()))
					_set_tile_explored(_game->state->get_player_pos());
				_update_automap = true;
				_reset_direction_indicator();
				_spinner_if();
				_teleport_if();
				_pit_if();
				_chute_if();
			}
			if (_exit_maze_now) {

				_display->set_disc(true);
				_refresh_display();
				_game->save_game();
				_display->set_disc(false);

				_exit_maze_now = false;
				return STOP_ENGINE;
			}
			_set_refresh_ui();
			_can_run_event = true;

		} else if (_system->input->check(CIN::CANCEL, event))
			_in_camp = true;
		else if (_system->input->check(CIN::BACK, event))
			_in_camp = true;
		else if (_system->input->check(CIN::CONFIRM, event)) {
			sf::Vector2f mouse_pos{_display->get_cur()};
			if (_party_panel->selected) {

				// Status-bar selected is 1-indexed, not 0-indexed
				const auto chosen{(_party_panel->selected.value())};
				if (auto result{_inspect->start(chosen)};
					result == MIM::ITEM_ABORT) {
					_inspect->stop();
					return STOP_ALL;
				} else {
					_inspect->stop();
					_party_panel->refresh();
					_display->generate("engine_base_ui");
					_display->set_input_mode(WIM::IN_GAME);
				}
			} else if (_left_icons->is_mouse_over(
						   (*_display
								   ->layout)["engine_base_ui:left_icon_panel"],
						   mouse_pos)) {

				if (std::optional<std::string> selected{
						_left_icons->set_mouse_selected(
							(*_display
									->layout)["engine_base_ui:left_icon_panel"],
							mouse_pos)};
					selected) {
					if (const auto &what(selected.value());
						what.ends_with("reorder")) {
						_party_panel->refresh();
						if (auto new_party{_reorder->start()}; new_party) {

							// TODO: handle aborts here too
							_game->state->set_party(new_party.value());

							_display->set_disc(true);
							_refresh_display();
							_game->save_game();
							_display->set_disc(false);

							_party_panel->refresh();
						}
						_reorder->stop();
						_party_panel->refresh();
						_display->generate("engine_base_ui");
						_display->set_input_mode(WIM::IN_GAME);

					} else if (what.ends_with("items")) {
						// TODO
					} else if (what.ends_with("achievements")) {
						// TODO
					} else if (what.ends_with("talk")) {
						// TODO
					} else if (what.ends_with("controls")) {
						_display->show_overlay();
						_system->set_pause(10000);
						_display->hide_overlay();

					} else if (what.ends_with("kick")) {
						// TODO
					} else if (what.ends_with("options")) {
						auto options{std::make_unique<Options>(
							_system, _display, _graphics)};
						if (auto result{options->start()}; result == STOP_ALL) {
							options->stop();
							return STOP_ALL;
						}
						options->stop();
						_party_panel->refresh();
						_display->generate("engine_base_ui");
						_display->set_input_mode(WIM::IN_GAME);
					} else if (what.ends_with("save")) {
						auto party{_game->state->get_party_characters()};
						for (auto &[character_id, character] :
							_game->characters) {
							if (std::find(party.begin(), party.end(),
									character_id) != party.end())
								character.set_location(CHL::MAZE);
						}
						_display->set_disc(true);
						_refresh_display();
						_game->save_game();
						_display->set_disc(false);

						_game->state->clear_party();
						return STOP_ENGINE;

					} else if (what.ends_with("exit")) {
						_show_confirm_exit = true;
						_display->set_input_mode(WIM::CONFIRM_QUIT_GAME);
						return CONTINUE;
					}
				}
			} else if (_right_icons->is_mouse_over(
						   (*_display
								   ->layout)["engine_base_ui:right_icon_panel"],
						   mouse_pos)) {

				if (std::optional<std::string> selected{
						_right_icons->set_mouse_selected(
							(*_display->layout)
								["engine_base_ui:right_icon_panel"],
							mouse_pos)};
					selected) {
					if (const auto &what{selected.value()};
						what.ends_with("left")) {
						_show_dir = true;
						_reset_direction_indicator();
						_turn_left();
						_spinner_if();
						_update_automap = true;
						_set_refresh_ui();
					} else if (what.ends_with("right")) {
						_show_dir = true;
						_reset_direction_indicator();
						_turn_right();
						_spinner_if();
						_set_refresh_ui();
					} else if (what.ends_with("forward")) {
						if (auto has_moved{_move_forward()}; !has_moved) {
							_show_dir = false;
							_show_ouch = true;
							_ouch->reset_timed();
						} else {
							_show_dir = true;
							_reset_direction_indicator();
							_teleport_if();
							_spinner_if();
							_pit_if();
							_chute_if();
							if (!_tile_explored(_game->state->get_player_pos()))
								_set_tile_explored(
									_game->state->get_player_pos());
							_update_automap = true;
						}
					} else if (what.ends_with("backward")) {
						if (auto has_moved{_move_backward()}; !has_moved) {
							_show_dir = false;
							_show_ouch = true;
							_ouch->reset_timed();
						} else {
							_show_dir = true;
							_reset_direction_indicator();
							_spinner_if();
							_teleport_if();
							_pit_if();
							_chute_if();
							if (!_tile_explored(_game->state->get_player_pos()))
								_set_tile_explored(
									_game->state->get_player_pos());
							_update_automap = true;
						}
					} else if (what.ends_with("camp")) {
						_in_camp = true;
						_set_refresh_ui();
						_display->set_input_mode(WIM::NAVIGATE_MENU);
					} else if (what.ends_with("use")) {
						// TODO
					} else if (what.ends_with("magic")) {
						// TODO
					} else if (what.ends_with("examine")) {
						// TODO
					} else if (what.ends_with("party")) {
						_party_panel->refresh();
						if (auto result{_inspect->start(std::nullopt)};
							result == MIM::ITEM_ABORT) {
							_inspect->stop();
							return STOP_ALL;
						}
						_inspect->stop();
						_party_panel->refresh();
						_display->generate("engine_base_ui");
						_display->set_input_mode(WIM::IN_GAME);
					}
				} else if (_right_icons->is_mouse_over(
							   (*_display->layout)["global:automap"],
							   mouse_pos)) {

					_in_map = true;
					_set_refresh_ui();

				} else if (_is_mouse_over(_search_bounds, mouse_pos)) {

					if (const auto others{_game->get_characters_at_loc()};
						!others.empty()) {
						_in_get = true;
						_get_menu->reload();
						_set_refresh_ui();
					}
				} else {

					// Otherwise the left click menu
					_in_action = true;
				}
			}
		} else if (_system->input->check(CIN::SHOW_HIDE_CONSOLE, event))
			_game->toggle_console();
		else if (_system->input->check(CIN::MOVE, event)) {

			// Check for Mouse Overs
			sf::Vector2f mouse_pos{_display->get_cur()};
			if (std::optional<std::string> selected{
					_left_icons->set_mouse_selected(
						(*_display->layout)["engine_base_ui:left_icon_panel"],
						mouse_pos)};
				selected) {
				_left_icons->selected = selected.value();
				if (_right_icons->selected)
					_right_icons->selected = std::nullopt;
				if (_party_panel->selected)
					_party_panel->selected = std::nullopt;
			}

			if (std::optional<std::string> selected{
					_right_icons->set_mouse_selected(
						(*_display->layout)["engine_base_ui:right_icon_"
											"panel"],
						mouse_pos)};
				selected) {
				_right_icons->selected = selected.value();
				if (_left_icons->selected)
					_left_icons->selected = std::nullopt;
				if (_party_panel->selected)
					_party_panel->selected = std::nullopt;
			}

			if (std::optional<unsigned int> selected{
					_party_panel->set_mouse_selected(mouse_pos)};
				selected) {
				_party_panel->selected = selected.value();
				_party_panel->refresh();
				if (_right_icons->selected)
					_right_icons->selected = std::nullopt;
				if (_left_icons->selected)
					_left_icons->selected = std::nullopt;
			}
		}
	}

	return std::nullopt;
}

auto Sorcery::Engine::_is_mouse_over(
	sf::IntRect rect, sf::Vector2f mouse_pos) const -> bool {

	return rect.contains(
		static_cast<int>(mouse_pos.x), static_cast<int>(mouse_pos.y));
}

// Entering the Maze
auto Sorcery::Engine::start() -> int {

	_initialise_state();
	_generate_display();
	_set_maze_entry_start();

	if (!_tile_explored(_game->state->get_player_pos()))
		_set_tile_explored(_game->state->get_player_pos());

	// Main Event Loops
	sf::Event event{};
	while (_window->isOpen()) {

		// Handle various timers
		_update_timers_and_components();
		_check_for_pending_events();

		if (_system->get_pause()) {

			_do_pause(event);
		} else {

			_pending_chute = false;
			_pending_elevator = false;
			_pending_combat = false;
			while (_window->pollEvent(event)) {

				// Check for Window Close
				if (event.type == sf::Event::Closed)
					return STOP_ALL;

				// Handle enabling help overlay
				if (_system->input->check(CIN::SHOW_CONTROLS, event)) {
					_display->show_overlay();
					continue;
				} else
					_display->hide_overlay();

				// Combat encounters are handled a bit differently
				if (const auto at_loc{_game->state->get_player_pos()};
					_game->state->level->at(at_loc).has_event()) {

					// Find the event and do something with it!
					const auto event{
						_game->state->level->at(at_loc).has_event().value()};
					switch (event) {
					case MAV::GUARANTEED_COMBAT:
						_pending_combat = true;
						break;
					case MAV::DEADLY_RING_COMBAT:
						_pending_combat = true;
						break;
					case MAV::FIRE_DRAGONS_COMBAT:
						_pending_combat = true;
						break;
					case MAV::WERDNA_COMBAT:
						_pending_combat = true;
						break;
					default:
						break;
					}
				}

				if (_show_confirm_search) {
					const auto do_search{_handle_confirm_search(event)};
					_display->set_input_mode(WIM::IN_GAME);
					if (do_search) {

						const auto at_loc{_game->state->get_player_pos()};
						if (_game->state->level->at(at_loc).has_event()) {

							// TODO: not sure this is used

							// Find the event and do something with it!
							const auto event{_game->state->level->at(at_loc)
									.has_event()
									.value()};
							if (event == MAV::MURPHYS_GHOSTS)
								_show_encounter = true;
						}
					}
				} else if (_show_confirm_exit) {
					_handle_confirm_exit(event);
				} else if (_in_map) {
					if (auto what_to_do{_handle_in_map(event)}; what_to_do) {
						if (what_to_do.value() == CONTINUE) {
							_in_map = false;
							continue;
						}
					}
				} else if (_in_camp) {
					if (auto what_to_do{_handle_in_camp(event)}; what_to_do) {
						if (what_to_do.value() == CONTINUE)
							continue;
						else if (what_to_do.value() == STOP_ENGINE)
							return STOP_ENGINE;
						else if (what_to_do.value() == STOP_ALL) {
							return STOP_ALL;
						}
					}
				} else if (_in_search) {
					if (auto what_to_do{_handle_in_search(event)}; what_to_do) {
						if (what_to_do.value() == CONTINUE)
							continue;
					}
				} else if (_in_get) {
					if (auto what_to_do{_handle_in_get(event)}; what_to_do) {
						if (what_to_do.value() == CONTINUE)
							continue;
					}
				} else if (_in_action) {
					if (auto what_to_do{_handle_in_action(event)}; what_to_do) {
						if (what_to_do.value() == CONTINUE)
							continue;
					}
				} else if (_in_elev_1) {
					if (auto what_to_do{_handle_elevator_a_d(event)};
						what_to_do) {
						if (what_to_do.value() == CONTINUE)
							continue;
					}
				} else if (_in_elev_2) {
					if (auto what_to_do{_handle_elevator_a_f(event)};
						what_to_do) {
						if (what_to_do.value() == CONTINUE)
							continue;
					}
				} else {

					if (_display->get_input_mode() == WIM::IN_GAME) {

						// Check for a wipe!
						if (_check_for_wipe())
							return _do_wipe();

						if (auto what_to_do{_handle_in_game(event)};
							what_to_do) {
							if (what_to_do.value() == CONTINUE)
								continue;
							else if (what_to_do.value() == STOP_ENGINE)
								return STOP_ENGINE;
							else if (what_to_do.value() == STOP_ALL) {
								return STOP_ALL;
							}
						}
					}
				}
			}
		}

		_update_display();
		_refresh_display();
	}

	return STOP_ENGINE;
}

auto Sorcery::Engine::_update_display() -> void {

	if (_update_render) {
		_render->refresh();
		_update_render = false;
	}
	if (_update_automap) {
		_automap->refresh();
		_map->refresh();
		_update_automap = false;
	}
	if (_update_compass) {
		_compass->refresh();
		_update_compass = false;
	}
	if (_update_buffbar) {
		_buffbar->refresh();
		_update_buffbar = false;
	}
	if (_update_debuffbar) {
		_debuffbar->refresh();
		_update_debuffbar = false;
	}
	if (_update_search) {
		_search->refresh();
		_update_search = false;
	}
	if (_update_icon_panels) {
		_left_icons->refresh(_in_camp);
		_right_icons->refresh(_in_camp);
		_update_icon_panels = false;
	}
	if (_update_party_panel) {
		_party_panel->refresh();
		_update_party_panel = false;
	}
}

auto Sorcery::Engine::_refresh_display() -> void {

	_window->clear();

	if (_display->layout->refresh_if_needed())
		_generate_display();

	_draw();
	_window->display();
}

// Remember Y is reversed
auto Sorcery::Engine::_move_forward() -> bool {

	auto at_loc{_game->state->get_player_pos()};
	auto x_d{at_loc.x};
	auto y_d{at_loc.y};

	switch (_game->state->get_player_facing()) {
	case MAD::NORTH:
		++y_d;
		break;
	case MAD::SOUTH:
		--y_d;
		break;
	case MAD::EAST:
		++x_d;
		break;
	case MAD::WEST:
		--x_d;
		break;
	default:
		break;
	}

	if (x_d < 0)
		x_d = MAP_SIZE - 1;
	else if (x_d > MAP_SIZE - 1)
		x_d = 0;
	if (y_d < 0)
		y_d = MAP_SIZE - 1;
	else if (y_d > MAP_SIZE - 1)
		y_d = 0;

	const auto next_loc{Coordinate{x_d, y_d}};
	auto this_tile{_game->state->level->at(at_loc)};
	const auto &next_tile{_game->state->level->at(next_loc)};

	if (const auto next_wall{_game->state->get_player_facing()};
		this_tile.walkable(next_wall)) {

		_game->state->set_player_prev_depth(_game->state->get_depth());
		_game->state->set_depth(_game->state->get_depth());
		_game->state->set_player_pos(next_loc);
		_can_go_back = true;

		if (!_tile_explored(_game->state->get_player_pos()))
			_set_tile_explored(_game->state->get_player_pos());
		if ((next_tile.is(TLP::DARKNESS)) && (_game->state->get_lit()))
			_game->state->set_lit(false);

		if (_game->state->level->stairs_at(next_loc)) {
			const auto at_loc{_game->state->get_player_pos()};
			if (const auto &to_tile{_game->state->level->at(at_loc)};
				to_tile.has(TLF::LADDER_UP))
				_confirm_stairs->set((
					*_display->layout)["engine_base_ui:dialog_ladder_up_text"]);
			else if (to_tile.has(TLF::LADDER_DOWN))
				_confirm_stairs->set((*_display
						->layout)["engine_base_ui:dialog_ladder_down_text"]);
			else if (to_tile.has(TLF::STAIRS_UP))
				_confirm_stairs->set((
					*_display->layout)["engine_base_ui:dialog_stairs_up_text"]);
			else if (to_tile.has(TLF::STAIRS_DOWN))
				_confirm_stairs->set((*_display
						->layout)["engine_base_ui:dialog_stairs_down_text"]);
			_show_confirm_stairs = true;
		} else
			_show_confirm_stairs = false;

		// TODO: clunky
		if (_game->state->level->elevator_at(next_loc)) {
			const auto elevator{
				_game->state->level->at(next_loc).has_elevator()};
			if (elevator.value().bottom_depth == -4) {
				_display->set_input_mode(WIM::NAVIGATE_MENU);
				MenuSelect opt_elev{_elev_1_menu->items.end()};
				_in_elev_1 = true;
			} else if (elevator.value().bottom_depth == -9) {
				_display->set_input_mode(WIM::NAVIGATE_MENU);
				MenuSelect opt_elev{_elev_2_menu->items.end()};
				_in_elev_2 = true;
			}
		} else {
			_in_elev_1 = false;
			_in_elev_2 = false;
		}

		_last_movement = MAD::NORTH; // Remember this is COMPASS direction (i.e.
									 // on screen), not Map direction

		return true;
	} else
		return false;
}

auto Sorcery::Engine::_move_backward() -> bool {

	// Work out our new position
	auto at_loc{_game->state->get_player_pos()};
	auto x_d{at_loc.x};
	auto y_d{at_loc.y};

	switch (_game->state->get_player_facing()) {
	case MAD::NORTH:
		--y_d;
		break;
	case MAD::SOUTH:
		++y_d;
		break;
	case MAD::EAST:
		--x_d;
		break;
	case MAD::WEST:
		++x_d;
		break;
	default:
		break;
	}

	if (x_d < 0)
		x_d = MAP_SIZE - 1;
	else if (x_d > MAP_SIZE - 1)
		x_d = 0;
	if (y_d < 0)
		y_d = MAP_SIZE - 1;
	else if (y_d > MAP_SIZE - 1)
		y_d = 0;

	const auto next_loc{Coordinate{x_d, y_d}};

	// Check for walls etc between current square and new square
	auto this_tile{_game->state->level->at(at_loc)};
	const auto &next_tile{_game->state->level->at(next_loc)};

	auto this_wall_to_check{MAD::NO_DIRECTION};
	switch (_game->state->get_player_facing()) {
	case MAD::NORTH:
		this_wall_to_check = MAD::SOUTH;
		break;
	case MAD::SOUTH:
		this_wall_to_check = MAD::NORTH;
		break;
	case MAD::EAST:
		this_wall_to_check = MAD::WEST;
		break;
	case MAD::WEST:
		this_wall_to_check = MAD::EAST;
		break;
	default:
		break;
	}

	if (this_tile.walkable(this_wall_to_check)) {

		_game->state->set_player_prev_depth(_game->state->get_depth());
		_game->state->set_depth(_game->state->get_depth());
		_game->state->set_player_pos(next_loc);
		_can_go_back = true;

		if (!_tile_explored(_game->state->get_player_pos()))
			_set_tile_explored(_game->state->get_player_pos());
		if ((next_tile.is(TLP::DARKNESS)) && (_game->state->get_lit()))
			_game->state->set_lit(false);

		if (_game->state->level->stairs_at(next_loc)) {
			const auto current_loc{_game->state->get_player_pos()};
			if (const auto &this_tile{_game->state->level->at(current_loc)};
				this_tile.has(TLF::LADDER_UP))
				_confirm_stairs->set((
					*_display->layout)["engine_base_ui:dialog_ladder_up_text"]);
			else if (this_tile.has(TLF::LADDER_DOWN))
				_confirm_stairs->set((*_display
						->layout)["engine_base_ui:dialog_ladder_down_text"]);
			else if (this_tile.has(TLF::STAIRS_UP))
				_confirm_stairs->set((
					*_display->layout)["engine_base_ui:dialog_stairs_up_text"]);
			else if (this_tile.has(TLF::STAIRS_DOWN))
				_confirm_stairs->set((*_display
						->layout)["engine_base_ui:dialog_stairs_down_text"]);
			_show_confirm_stairs = true;
		} else
			_show_confirm_stairs = false;

		// TODO:clunky
		if (_game->state->level->elevator_at(next_loc)) {
			const auto elevator{
				_game->state->level->at(next_loc).has_elevator()};
			if (elevator.value().bottom_depth == -4) {
				_display->set_input_mode(WIM::NAVIGATE_MENU);
				MenuSelect opt_elev{_elev_1_menu->items.end()};
				_in_elev_1 = true;
			} else if (elevator.value().bottom_depth == -9) {
				_display->set_input_mode(WIM::NAVIGATE_MENU);
				MenuSelect opt_elev{_elev_2_menu->items.end()};
				_in_elev_2 = true;
			}
		} else {
			_in_elev_1 = false;
			_in_elev_2 = false;
		}

		_last_movement = MAD::SOUTH;

		return true;
	} else
		return false;
}

auto Sorcery::Engine::_turn_left() -> void {

	switch (_game->state->get_player_facing()) {
	case MAD::NORTH:
		_game->state->set_player_facing(MAD::WEST);
		break;
	case MAD::SOUTH:
		_game->state->set_player_facing(MAD::EAST);
		break;
	case MAD::EAST:
		_game->state->set_player_facing(MAD::NORTH);
		break;
	case MAD::WEST:
		_game->state->set_player_facing(MAD::SOUTH);
		break;
	default:
		break;
	}

	_last_movement = MAD::WEST;
	_can_go_back = false;
}

auto Sorcery::Engine::_turn_right() -> void {

	switch (_game->state->get_player_facing()) {
	case MAD::NORTH:
		_game->state->set_player_facing(MAD::EAST);
		break;
	case MAD::SOUTH:
		_game->state->set_player_facing(MAD::WEST);
		break;
	case MAD::EAST:
		_game->state->set_player_facing(MAD::SOUTH);
		break;
	case MAD::WEST:
		_game->state->set_player_facing(MAD::NORTH);
		break;
	default:
		break;
	}

	_last_movement = MAD::EAST;
	_can_go_back = false;
}

auto Sorcery::Engine::_turn_around() -> void {

	switch (_game->state->get_player_facing()) {
	case MAD::NORTH:
		_game->state->set_player_facing(MAD::SOUTH);
		break;
	case MAD::SOUTH:
		_game->state->set_player_facing(MAD::NORTH);
		break;
	case MAD::EAST:
		_game->state->set_player_facing(MAD::WEST);
		break;
	case MAD::WEST:
		_game->state->set_player_facing(MAD::EAST);
		break;
	default:
		break;
	}

	_last_movement = MAD::SOUTH;
	_can_go_back = false;
}

// TODO: rock/walkable for all levels/tiles!
auto Sorcery::Engine::_pit_if() -> bool {

	if (const auto tile{
			_game->state->level->at(_game->state->get_player_pos())};
		tile.has(TLF::PIT)) {
		_show_pit = true;
		_pit_oops();
		_update_party_panel = true;
		_pit->set_valid(true);
		_pit->reset_timed();
		return true;
	}

	return false;
}

auto Sorcery::Engine::_combat_if() -> bool {

	if (_next_combat) {

		// Wait until the encounter window has disappeared then initialise
		// combat
		if (!_encounter->get_valid()) {

			// do combat
			PRINT("Combat Here!");
			_next_combat = std::nullopt;
			return true;
		}
	}

	return false;
}

auto Sorcery::Engine::_encounter_if() -> bool {

	if (_show_encounter) {
		_encounter->set_valid(true);
		_encounter->reset_timed();
		return true;
	}

	return false;
}

// Example of dice rolling
auto Sorcery::Engine::_pit_oops() -> void {

	std::vector<int> deaths{};
	deaths.clear();

	auto party{_game->state->get_party_characters()};
	for (auto &[id, character] : _game->characters) {
		if (std::find(party.begin(), party.end(), id) != party.end()) {

			const auto chance{(character.get_cur_attr(CAR::AGILITY) -
								  _game->state->get_depth()) *
							  4};
			const auto roll((*_system->random)[RNT::D100]);
			_game->state->add_log_dice_roll(
				fmt::format("{:>16} - {}", character.get_name(), "Avoid Pit"),
				100, roll, chance);
			if (roll < chance) {

				// Damage is avoided

			} else {

				// Now in the original Apple 2 version, pit damage is based upon
				// 3 extra values stored in the square in the TMaze records -
				// AUX0, AUX1, and AUX2. Thanks to the data extraction by Tommy
				// Ewers, the relevant values for each pit in the game are 0, 8
				// and depth respectively. This is a long-winded way of saying
				// that the pit damage (calculated in APIT and ROCKWATR) is 0 +
				// (depth * d8), i.e. a d8 for level depth.

				// Inflict damage! (remember depth is negative here and positve
				// in original wizardry)
				auto pit_damage{0U};
				const auto dice{std::abs(_game->state->get_depth())};
				for (int i = 1; i <= dice; i++)
					pit_damage += (*_system->random)[RNT::D8];

				_game->state->add_log_message(
					fmt::format(
						"{} fell into a pit and took {} points of damage!",
						character.get_name(), pit_damage),
					IMT::GAME);

				if (const auto alive{character.damage(pit_damage)}; !alive) {

					// Oh dear, death from a pit!
					_game->state->add_log_message(
						fmt::format("{} has died!", character.get_name()),
						IMT::GAME);
					deaths.emplace_back(id);
				}
			}
		}
	}

	if (!deaths.empty()) {

		// need to display a character has died dialog
	}
}

auto Sorcery::Engine::_event_if() -> bool {

	// If we are in-game, and are on something that will happen - note that
	// consequences of these events are dealt with otherwise in the main loop
	// when the various flags are set. Note that all events initially take the
	// form of a popup screen with info on it and a continue button
	if (const auto at{_game->state->get_player_pos()};
		_game->state->level->at(at).has_event() && _can_run_event) {

		const auto event_type{_game->state->level->at(at).has_event().value()};
		const auto dungeon_event{_game->get_event(event_type)};

		// First check the game quest flags (TODO - inventory items instead) -
		// if we have the necessary item then we don't need to run the event at
		// all
		switch (event_type) {
		case MAV::NEED_BEAR_STATUE:
			[[fallthrough]];
		case MAV::NEED_BEAR_STATUE_2:
			if (_game->state->quest_item_flags[unenum(ItemQuest::BEAR_STATUE)])
				return true;
			break;
		case MAV::NEED_FROG_STATUE:
			if (_game->state->quest_item_flags[unenum(ItemQuest::FROG_STATUE)])
				return true;
			break;
		case MAV::NEED_SILVER_KEY:
			if (_game->state->quest_item_flags[unenum(ItemQuest::SILVER_KEY)])
				return true;
			break;
		case MAV::NEED_BRONZE_KEY:
			if (_game->state->quest_item_flags[unenum(ItemQuest::BRONZE_KEY)])
				return true;
			break;
		case MAV::NEED_BLUE_RIBBON:
			if (_game->state->quest_item_flags[unenum(ItemQuest::BLUE_RIBBON)])
				return true;
			break;

		default:
			break;
		}

		_show_dir = false;
		_show_ouch = false;
		_display_cursor = false;
		_refresh_display();

		if (_can_run_event) {
			if (event_type == MAV::TREBOR_VOICE) {

				// Linked events
				auto event_1{std::make_unique<Event>(
					_system, _display, _graphics, _game, event_type, 1)};
				if (auto result{event_1->start()}; result == MIM::ITEM_ABORT) {
					event_1->stop();
					_can_run_event = false;
					_display_cursor = true;
					_refresh_display();
					return STOP_ALL;
				} else {
					event_1->stop();

					_refresh_display();
					auto event_2{std::make_unique<Event>(
						_system, _display, _graphics, _game, event_type, 2)};
					if (auto result{event_2->start()};
						result == MIM::ITEM_ABORT) {

						_can_run_event = false;
						_display_cursor = true;
						_refresh_display();
						return STOP_ALL;
					}
					event_2->stop();

					_show_found = true;

					// random character who has inventory free
					const auto &character{_game->characters[_game->state
							->get_character_by_position(1)
							.value()]};
					const auto text{fmt::format("{}{}", character.get_name(),
						(*_display->string)["FOUND_AN_ITEM"])};
					_found->set(
						(*_display->layout)["engine_base_ui:found_an_item"],
						text);
					_found->reset_timed();

					if (!_game->state
							->quest_item_flags[unenum(ItemQuest::BLUE_RIBBON)])
						_game->state
							->quest_item_flags[unenum(ItemQuest::BLUE_RIBBON)] =
							true;
				}

			} else if (event_type == MAV::WERDNA_BOAST) {

				// Linked events
				// Linked events
				auto event_1{std::make_unique<Event>(
					_system, _display, _graphics, _game, event_type, 1)};
				if (auto result{event_1->start()}; result == MIM::ITEM_ABORT) {
					event_1->stop();
					_can_run_event = false;
					_display_cursor = true;
					_refresh_display();
					return STOP_ALL;
				} else {
					event_1->stop();

					_refresh_display();
					auto event_2{std::make_unique<Event>(
						_system, _display, _graphics, _game, event_type, 2)};
					if (auto result{event_2->start()};
						result == MIM::ITEM_ABORT) {

						_can_run_event = false;
						_display_cursor = true;
						_refresh_display();
						return STOP_ALL;
					} else {
						event_2->stop();

						_refresh_display();
						auto event_3{std::make_unique<Event>(_system, _display,
							_graphics, _game, event_type, 3)};
						if (auto result{event_3->start()};
							result == MIM::ITEM_ABORT) {

							_can_run_event = false;
							_display_cursor = true;
							_refresh_display();
							return STOP_ALL;
						}
						event_3->stop();
					}
				}

			} else {

				auto event{std::make_unique<Event>(
					_system, _display, _graphics, _game, event_type)};
				if (auto result{event->start()}; result == MIM::ITEM_ABORT) {
					event->stop();
					_can_run_event = false;
					_display_cursor = true;
					_refresh_display();
					return STOP_ALL;
				} else {
					event->stop();
				}
			}
		}

		// Handle what happens after this TODO - chained events
		_show_confirm_search = false;
		if (dungeon_event.search_after) {
			_show_confirm_search = true;
			_display->set_input_mode(WIM::CONFIRM_QUIT_GAME);
		} else if (dungeon_event.go_town_after)
			_exit_maze_now = true;
		else if (dungeon_event.go_back_after) {
			_go_back();
			_update_automap = true;
			_set_refresh_ui();
		} else if (dungeon_event.combat_after) {

			// do combat (need combat event with an optional combat type -
			// DEADLY RING, FIREDRAGONS, WERDNA, RANDOM)
			std::cout << "An Encounter!" << std::endl;
		}

		_can_run_event = false;
		_display_cursor = true;
		_refresh_display();
	}

	return false;
}

auto Sorcery::Engine::_elevator_if() -> bool {

	if (const auto tile{
			_game->state->level->at(_game->state->get_player_pos())};
		tile.has(TLF::ELEVATOR)) {
		_show_elevator = true;
		_elevator->set_valid(true);
		_elevator->reset_timed();
		_system->set_pause(2000);
		_pending_elevator = true;

		return true;
	}

	return false;
}

auto Sorcery::Engine::_chute_if() -> bool {

	if (const auto tile{
			_game->state->level->at(_game->state->get_player_pos())};
		tile.has(TLF::CHUTE)) {
		_show_chute = true;
		_chute->set_valid(true);
		_chute->reset_timed();
		_system->set_pause(2000);
		_pending_chute = true;

		return true;
	}

	return false;
}

auto Sorcery::Engine::_spinner_if() const -> bool {

	if (const auto tile{
			_game->state->level->at(_game->state->get_player_pos())};
		tile.has(TLF::SPINNER)) {

		// Random Direction Change
		auto new_facing{static_cast<MAD>((*_system->random)[RNT::ZERO_TO_3])};
		_game->state->set_player_facing(new_facing);
		return true;
	}

	return false;
}

auto Sorcery::Engine::_stairs_if() -> bool {

	if (const auto tile{
			_game->state->level->at(_game->state->get_player_pos())};
		tile.has_stairs()) {

		auto destination{tile.has_stairs().value()};
		auto to_level{destination.to_level};

		// Floors are negative
		if (to_level < 0) {
			Level level{((*_game->levelstore)[to_level]).value()};
			_game->state->set_current_level(&level);
			_game->state->set_player_pos(destination.to_loc);
			_game->state->set_player_prev_depth(_game->state->get_depth());
			_game->state->set_depth(to_level);
			_set_tile_explored(_game->state->get_player_pos());
			_can_go_back = true;

			const auto &next_tile{_game->state->level->at(destination.to_loc)};
			if ((next_tile.is(TLP::DARKNESS)) && (_game->state->get_lit()))
				_game->state->set_lit(false);

			if (next_tile.has(TLF::LADDER_UP))
				_confirm_stairs->set((
					*_display->layout)["engine_base_ui:dialog_ladder_up_text"]);
			else if (next_tile.has(TLF::LADDER_DOWN))
				_confirm_stairs->set((*_display
						->layout)["engine_base_ui:dialog_ladder_down_text"]);
			else if (next_tile.has(TLF::STAIRS_UP))
				_confirm_stairs->set((
					*_display->layout)["engine_base_ui:dialog_stairs_up_text"]);
			else if (next_tile.has(TLF::STAIRS_DOWN))
				_confirm_stairs->set((*_display
						->layout)["engine_base_ui:dialog_stairs_down_text"]);

			return true;
		}
	}

	return false;
}

auto Sorcery::Engine::_teleport_if() -> bool {

	// TODO: handle anti-teleport here in the future
	if (const auto tile{
			_game->state->level->at(_game->state->get_player_pos())};
		tile.has_teleport()) {

		auto destination{tile.has_teleport().value()};
		if (destination.to_level == 0) {

			// Special case of teleporting back to castle
			_exit_maze_now = true;
			return true;
		} else if (destination.to_level == _game->state->get_depth()) {

			const auto &next_tile{_game->state->level->at(destination.to_loc)};
			_game->state->set_player_prev_depth(_game->state->get_depth());
			_game->state->set_depth(_game->state->get_depth());
			_game->state->set_player_pos(destination.to_loc);
			_can_go_back = true;

			if (!_tile_explored(_game->state->get_player_pos()))
				_set_tile_explored(_game->state->get_player_pos());

			if ((next_tile.is(TLP::DARKNESS)) && (_game->state->get_lit()))
				_game->state->set_lit(false);

			if (_game->state->level->stairs_at(
					_game->state->get_player_pos())) {
				const auto current_loc{_game->state->get_player_pos()};
				if (const auto &at{_game->state->level->at(current_loc)};
					at.has(TLF::LADDER_UP))
					_confirm_stairs->set((*_display
							->layout)["engine_base_ui:dialog_ladder_up_text"]);
				else if (at.has(TLF::LADDER_DOWN))
					_confirm_stairs->set((*_display->layout)
							["engine_base_ui:dialog_ladder_down_text"]);
				else if (at.has(TLF::STAIRS_UP))
					_confirm_stairs->set((*_display
							->layout)["engine_base_ui:dialog_stairs_up_text"]);
				else if (at.has(TLF::STAIRS_DOWN))
					_confirm_stairs->set((*_display->layout)
							["engine_base_ui:dialog_stairs_down_text"]);
				_show_confirm_stairs = true;
			} else
				_show_confirm_stairs = false;
		} else {

			// TODO: cross teleport levels
		}
	}

	return true;
}

auto Sorcery::Engine::stop() const -> void {

	// TODO
}

auto Sorcery::Engine::_draw() -> void {

	_graphics->tile_bg(_window);

	// Standard Components
	_display->display("engine_base_ui");

	// Draw and scale the Render
	if (_show_gui) {

		_window->draw(*_view_frame_small);

		const auto wfr_c{
			(*_display->layout)["engine_base_ui:wireframe_view_small"]};
		_render->setScale(std::stof(wfr_c["scale_x"].value()),
			std::stof(wfr_c["scale_y"].value()));
		_window->draw(*_view_frame_small);
		_render->setPosition(wfr_c.x + std::stoi(wfr_c["offset_x"].value()),
			wfr_c.y + std::stoi(wfr_c["offset_y"].value()));
		_window->draw(*_render);

	} else {

		_window->draw(*_view_frame_big);

		const auto wfr_c{
			(*_display->layout)["engine_base_ui:wireframe_view_big"]};
		_render->setScale(std::stof(wfr_c["scale_x"].value()),
			std::stof(wfr_c["scale_y"].value()));
		_window->draw(*_view_frame_big);
		_render->setPosition(wfr_c.x + std::stoi(wfr_c["offset_x"].value()),
			wfr_c.y + std::stoi(wfr_c["offset_y"].value()));
		_window->draw(*_render);
	}

	if (_show_gui) {

		_window->draw(*_automap);
		_window->draw(*_compass);
		_window->draw(*_buffbar);
		_window->draw(*_debuffbar);
		_window->draw(*_search);

		if (_left_icons->selected)
			_left_icons->set_selected_background();
		if (_right_icons->selected)
			_right_icons->set_selected_background();

		_window->draw(*_left_icons);
		_window->draw(*_right_icons);
	}

	if (_show_party_panel)
		_window->draw(*_party_panel);

	if (_in_map) {
		_window->draw(*_map);
	} else if (_in_camp) {
		_window->draw(*_camp_menu_frame);
		_camp_menu->generate((*_display->layout)["engine_base_ui:camp_menu"]);
		_window->draw(*_camp_menu);
	} else if (_in_action) {
		_window->draw(*_action_menu_frame);
		_action_menu->generate(
			(*_display->layout)["engine_base_ui:action_menu"]);
		_window->draw(*_action_menu);
	} else if (_in_search) {
		_window->draw(*_search_menu_frame);
		_search_menu->generate(
			(*_display->layout)["engine_base_ui:search_menu"]);
		_window->draw(*_search_menu);
	} else if (_in_get) {
		_window->draw(*_get_menu_frame);
		_get_menu->generate((*_display->layout)["engine_base_ui:get_menu"]);
		_window->draw(*_get_menu);
	} else if (_in_elev_1) {
		_window->draw(*_elev_1_menu_frame);
		_elev_1_menu->generate(
			(*_display->layout)["engine_base_ui:elevator_a_d_menu"]);
		_window->draw(*_elev_1_menu);

		if (_show_elevator) {
			if (_elevator->get_valid())
				_window->draw(*_elevator);
			else {
				_show_elevator = false;
				_elevator->set_valid(false);
			}
		}
	} else if (_in_elev_2) {
		_window->draw(*_elev_2_menu_frame);
		_elev_2_menu->generate(
			(*_display->layout)["engine_base_ui:elevator_a_f_menu"]);
		_window->draw(*_elev_2_menu);

		if (_show_elevator) {
			if (_elevator->get_valid())
				_window->draw(*_elevator);
			else {
				_show_elevator = false;
				_elevator->set_valid(false);
			}
		}
	} else if (_display->get_input_mode() == WIM::BROWSE_CHARACTER) {
		if (_cur_char) {

			// If we have a character
			_window->draw(*_cur_char_frame);
			_character_display->setPosition(
				(*_display->layout)["engine_base_ui:character"].pos());
			_character_display->update();
			_window->draw(*_character_display);

			if (_show_pool) {
				_pool->update();
				_window->draw(*_pool);
			}
		}
	} else if (_display->get_input_mode() == WIM::IN_GAME) {

		if (_show_confirm_stairs) {
			_confirm_stairs->update();
			_window->draw(*_confirm_stairs);
		}

		if (_show_confirm_search) {
			_confirm_search->update();
			_window->draw(*_confirm_search);
		}

		if (_show_ouch) {
			if (_ouch->get_valid())
				_window->draw(*_ouch);
			else {
				_show_ouch = false;
				_ouch->set_valid(false);
			}
		}

		if (_show_encounter) {
			if (_encounter->get_valid())
				_window->draw(*_encounter);
			else {
				_show_encounter = false;
				_encounter->set_valid(false);
			}
		}

		if (_show_found) {
			if (_found->get_valid())
				_window->draw(*_found);
			else {
				_show_found = false;
				_found->set_valid(false);
			}
		}

		if (_show_pit) {
			if (_pit->get_valid())
				_window->draw(*_pit);
			else {
				_show_pit = false;
				_pit->set_valid(false);
			}
		}

		if (_show_chute) {
			if (_chute->get_valid())
				_window->draw(*_chute);
			else {
				_show_chute = false;
				_chute->set_valid(false);
			}
		}
	}

	if (_show_dir) {
		_display->display_direction_indicator(_last_movement, _monochrome);
	}

	if (_show_confirm_exit) {
		_confirm_exit->update();
		_window->draw(*_confirm_exit);
	}

	// Always draw the following
	_display->display_overlay();
	if (_display_cursor)
		_display->display_cursor();

	if (_game->get_console_status()) {
		_console->refresh();
		_display->window->get_gui()->draw();
	}

	_display->window->draw_cursor_coord(_display->get_cur());
}

auto Sorcery::Engine::_go_back() -> std::optional<int> {

	if (_can_go_back) {

		const auto current_depth{_game->state->get_depth()};
		const auto previous_depth{_game->state->get_player_prev_depth()};
		const auto previous_pos{_game->state->get_player_prev_pos()};
		if (current_depth == previous_depth) {
			_game->state->set_player_pos(previous_pos);
			_can_go_back = false;
		} else {
			if (previous_depth == 0) {

				_unpoison_characters();
				_triage_characters();

				_display->set_disc(true);
				_refresh_display();
				_game->save_game();
				_display->set_disc(false);
				_can_go_back = false;

				return STOP_ENGINE;
			} else {
				Level level{((*_game->levelstore)[previous_depth]).value()};
				_game->state->set_current_level(&level);
				_game->state->set_depth(previous_depth);
				_game->state->set_player_prev_depth(_game->state->get_depth());
				_game->state->set_player_pos(previous_pos);
				_can_go_back = false;
			}
		}
	}

	return CONTINUE;
}

auto Sorcery::Engine::_set_refresh_ui() -> void {

	_update_automap = true;
	_update_compass = true;
	_update_buffbar = true;
	_update_debuffbar = true;
	_update_search = true;
	_update_render = true;
}

// Various Debug Functions - can be placed in _handle_in_game and associated
// with keypresses
auto Sorcery::Engine::_debug_go_back() -> std::optional<int> {

	_go_back();

	_set_refresh_ui();

	return CONTINUE;
}

auto Sorcery::Engine::_debug_start_random_combat() -> std::optional<int> {

	_show_encounter = true;
	_next_combat = CombatType::RANDOM;

	return std::nullopt;
}

auto Sorcery::Engine::_debug_set_quest_item_flags() -> std::optional<int> {

	std::fill(_game->state->quest_item_flags.begin(),
		_game->state->quest_item_flags.end(), true);

	return CONTINUE;
}

auto Sorcery::Engine::_debug_clear_quest_item_flags() -> std::optional<int> {

	std::fill(_game->state->quest_item_flags.begin(),
		_game->state->quest_item_flags.end(), false);

	return CONTINUE;
}

auto Sorcery::Engine::_debug_go_to_graveyard() -> std::optional<int> {

	_graveyard->start();
	_graveyard->stop();

	return STOP_ENGINE;
}

auto Sorcery::Engine::_debug_go_down_a_level() -> std::optional<int> {

	auto dest_level{_game->state->get_depth() + 1};
	Level level{((*_game->levelstore)[dest_level]).value()};
	_game->state->set_current_level(&level);
	_game->state->set_player_prev_depth(_game->state->get_depth());
	_game->state->set_depth(dest_level);
	_set_tile_explored(_game->state->get_player_pos());

	_set_refresh_ui();

	return CONTINUE;
}

auto Sorcery::Engine::_debug_go_up_a_level() -> std::optional<int> {

	auto dest_level{_game->state->get_depth() - 1};
	Level level{((*_game->levelstore)[dest_level]).value()};
	_game->state->set_current_level(&level);
	_game->state->set_player_prev_depth(_game->state->get_depth());
	_game->state->set_depth(dest_level);
	_set_tile_explored(_game->state->get_player_pos());

	_set_refresh_ui();

	return CONTINUE;
}

auto Sorcery::Engine::_debug_heal_party_to_full() -> std::optional<int> {

	const auto party{_game->state->get_party_characters()};
	for (auto &[id, character] : _game->characters) {
		if (std::find(party.begin(), party.end(), id) != party.end()) {
			character.set_status(CHT::OK);
			character.set_current_hp(character.get_max_hp());
			character.reset_adjustment_per_turn();
			character.set_poisoned_rate(0);
		}
	}

	_set_refresh_ui();

	_update_party_panel = true;

	return CONTINUE;
}

auto Sorcery::Engine::_debug_give_first_character_gold_xp()
	-> std::optional<int> {

	auto &character{
		_game->characters[_game->state->get_character_by_position(1).value()]};
	auto next{character.get_next_xp()};
	character.grant_xp(next - 1);
	character.grant_gold(10000);

	_set_refresh_ui();

	_update_party_panel = true;

	return CONTINUE;
}

auto Sorcery::Engine::_debug_level_first_character_down()
	-> std::optional<int> {

	auto &character{
		_game->characters[_game->state->get_character_by_position(1).value()]};
	character.level_down();

	_set_refresh_ui();

	_update_party_panel = true;

	return CONTINUE;
}

auto Sorcery::Engine::_debug_level_first_character_up() -> std::optional<int> {

	auto &character{
		_game->characters[_game->state->get_character_by_position(1).value()]};
	auto next{character.get_next_xp()};
	character.grant_xp(next + 1);

	_set_refresh_ui();

	_update_party_panel = true;

	return CONTINUE;
}

auto Sorcery::Engine::_debug_kill_non_party_characters() -> std::optional<int> {

	for (auto &[id, character] : _game->characters) {
		if (character.get_location() != CHL::PARTY) {
			character.set_current_hp(0);
			character.set_status(CHT::DEAD);
			character.set_location(CHL::TEMPLE);
		}
	}

	_set_refresh_ui();

	_update_party_panel = true;

	return CONTINUE;
}

auto Sorcery::Engine::_debug_send_non_party_characters_to_tavern()
	-> std::optional<int> {

	for (auto &[id, character] : _game->characters) {
		if (character.get_location() != CHL::PARTY) {
			character.set_current_hp(character.get_max_hp());
			character.set_status(CHT::OK);
			character.set_location(CHL::TAVERN);
		}
	}

	_set_refresh_ui();

	_update_party_panel = true;

	return CONTINUE;
}

auto Sorcery::Engine::_debug_give_party_random_hp() -> std::optional<int> {

	auto party{_game->state->get_party_characters()};
	for (auto &[id, character] : _game->characters) {
		if (std::find(party.begin(), party.end(), id) != party.end()) {
			character.set_current_hp(1);
			if ((*_system->random)[RNT::ZERO_TO_2] == 0)
				character.set_current_hp(1);
			else
				character.set_current_hp(character.get_max_hp());
		}
	}

	_set_refresh_ui();

	_update_party_panel = true;

	return CONTINUE;
}

auto Sorcery::Engine::_debug_give_party_random_status() -> std::optional<int> {

	const auto party{_game->state->get_party_characters()};
	for (auto &[id, character] : _game->characters) {
		if (std::find(party.begin(), party.end(), id) != party.end()) {
			character.set_status(
				magic_enum::enum_cast<CHT>((*_system->random)[RNT::ZERO_TO_8])
					.value());
			if ((character.get_status() == CHT::DEAD) ||
				(character.get_status() == CHT::ASHES) ||
				(character.get_status() == CHT::LOST)) {
				character.set_current_hp(0);
			} else
				character.set_current_hp(character.get_max_hp());
		}
	}

	_set_refresh_ui();

	_update_party_panel = true;

	return CONTINUE;
}

auto Sorcery::Engine::_debug_light_on() -> std::optional<int> {

	_game->state->set_lit(true);

	_set_refresh_ui();

	return CONTINUE;
}

auto Sorcery::Engine::_debug_light_off() -> std::optional<int> {

	_game->state->set_lit(false);

	_set_refresh_ui();

	return CONTINUE;
}

auto Sorcery::Engine::_debug_monochrome_wireframe() -> std::optional<int> {

	_render->set_monochrome(true);

	_set_refresh_ui();

	return CONTINUE;
}

auto Sorcery::Engine::_debug_colour_wireframe() -> std::optional<int> {

	_render->set_monochrome(false);

	_set_refresh_ui();

	return CONTINUE;
}
