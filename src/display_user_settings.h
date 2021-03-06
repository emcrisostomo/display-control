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

#ifndef DISPLAY_CONTROL_DISPLAY_USER_SETTINGS_H
#define DISPLAY_CONTROL_DISPLAY_USER_SETTINGS_H

#include <memory>
#include <unordered_map>

namespace emc
{
  class display_user_settings
  {
  public:
    static display_user_settings load();

    display_user_settings();
    display_user_settings(display_user_settings&& other) noexcept = default;
    display_user_settings& operator=(display_user_settings&& other) noexcept = default;
    display_user_settings(const display_user_settings&) = delete;
    display_user_settings& operator=(const display_user_settings&) = delete;
    ~display_user_settings();

    void clear();
    void set_display_brightness(unsigned long display, float brightness);
    float get_display_brightness(unsigned long display) const;
    bool has_configuration(unsigned long display) const;
    void save();

  private:
    std::unordered_map<unsigned long, float> brightness_settings;
  };
}

#endif //DISPLAY_CONTROL_DISPLAY_USER_SETTINGS_H
