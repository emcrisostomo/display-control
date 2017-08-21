/*
 * Copyright (c) 2017 Enrico M. Crisostomo
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "display_user_settings.h"
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <pwd.h>
#include <stdexcept>
#include <fstream>
#include <regex>
#include "gettext_defs.h"
#include "cmake_config.h"

namespace emc
{
  static std::string get_data_home_dir();
  static bool parse_settings(std::string settings,
                             std::pair<unsigned int, float>& pair,
                             void (*err_handler)(std::string));

  std::string get_data_home_dir()
  {
    const char *xdg_data_home = getenv("XDG_DATA_HOME");
    if (xdg_data_home != nullptr) return xdg_data_home;

    const char *home = getenv("HOME");
    if (home != nullptr) return std::string(home) + "/.local/share";;

    struct passwd pwd{};
    struct passwd *result;

    long buffer_size = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (buffer_size == -1) buffer_size = 16384;

    auto buffer = new char[buffer_size];

    auto ret = getpwuid_r(getuid(), &pwd, buffer, static_cast<size_t>(buffer_size), &result);
    if (ret != 0)
    {
      throw std::runtime_error(_("Couldn't retrieve user home directory"));
    }

    if (result == nullptr)
    {
      throw std::runtime_error(strerror(ret));
    }

    return std::string(result->pw_dir) + "/.local/share";
  }

  bool parse_settings(std::string settings, std::pair<unsigned int, float>& pair, void (*err_handler)(std::string))
  {
#define handle_error(t) if (err_handler) err_handler(t);
    // Skip empty strings.
    if (settings.length() == 0) return false;

    // Strip comments.
    if (settings[0] == '#') return false;

    // Settings have the following structure:
    // x:b
    // where:
    //   * x is the display index (x >= 0)
    //   * b is the display brightness (0 <= b <= 1)
    static std::regex filter_grammar(R"(^(\d+):(\d*\.?\d*) *$)", std::regex_constants::ECMAScript);
    std::smatch fragments;

    if (!regex_match(settings, fragments, filter_grammar))
    {
      handle_error(settings);
      return false;
    }

    try
    {
      unsigned long display_number = std::stoul(fragments[1].str(), nullptr, 10);
      float display_brightness = std::stof(fragments[2].str(), nullptr);

      pair = std::make_pair(display_number, display_brightness);
      return true;
    }
    catch (std::logic_error& ex)
    {
      std::cerr << _("Invalid display settings: ") << fragments[0] << "\n";
      return false;
    }
  }

  display_user_settings display_user_settings::load()
  {
    std::string home = get_data_home_dir();
    std::string config_file_path = home + "/" + PACKAGE + "/displays";
    std::ifstream f(config_file_path);

    if (!f.is_open()) return {};

    std::string line;
    std::pair<unsigned int, float> pair;
    display_user_settings settings;

    while (std::getline(f, line))
    {
      if (parse_settings(line,
                         pair,
                         [](std::string s)
                         {
                           std::cerr << _("Invalid display settings: ") << s << "\n";
                         }))
      {
        settings.brightness_settings.insert(pair);
      }
    }

    return settings;
  }

  void display_user_settings::clear()
  {
    brightness_settings.clear();
  }

  void display_user_settings::set_display_brightness(unsigned int display, float brightness)
  {
    brightness_settings[display] = brightness;
  }

  float display_user_settings::get_display_brightness(unsigned int display) const
  {
    if (!has_configuration(display))
    {
      std::string err = _("The specified display index has no settings: ") + std::to_string(display);
      throw std::out_of_range(err);
    }

    return brightness_settings.at(display);
  }

  bool display_user_settings::has_configuration(unsigned int display) const
  {
    return (brightness_settings.find(display) != brightness_settings.end());
  }

  void display_user_settings::save()
  {
    std::string home = get_data_home_dir();
    std::string config_file_path = home + "/" + PACKAGE + "/displays";
    std::ofstream f(config_file_path, std::ios::trunc);

    for (const auto& p : brightness_settings) f << p.first << ":" << p.second << "eee\n";
  }

  display_user_settings::display_user_settings() = default;

  display_user_settings::~display_user_settings() = default;
}