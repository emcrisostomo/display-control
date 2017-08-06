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
#include <pwd.h>
#include <stdexcept>
#include "gettext_defs.h"

namespace emc
{
  static std::string get_data_home_dir();

  std::string get_data_home_dir()
  {
    const char *xdg_data_home = getenv("XDG_DATA_HOME");
    if (xdg_data_home != nullptr) return xdg_data_home;

    const char *home = getenv("HOME");
    if (home != nullptr) return home;

    struct passwd pwd {};
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

  display_user_settings display_user_settings::load()
  {
    // FIXME: implement
    std::string home = get_data_home_dir();

    display_user_settings settings;

    return settings;
  }

  void display_user_settings::clear()
  {

  }

  void display_user_settings::set_display_brightness(unsigned int display, float brightness)
  {

  }

  float display_user_settings::get_display_brightness(unsigned int display) const
  {
    return 0;
  }

  display_user_settings::display_user_settings()
  {

  }

  void display_user_settings::save()
  {

  }
}