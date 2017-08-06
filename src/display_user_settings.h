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

namespace emc
{
  class display_user_settings
  {
  public:
    static display_user_settings load();
    void clear();
    void set_display_brightness(unsigned int display, float brightness);
    float get_display_brightness(unsigned int display) const;
  private:
    display_user_settings();
  };
}

#endif //DISPLAY_CONTROL_DISPLAY_USER_SETTINGS_H
