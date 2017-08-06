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

emc::display_user_settings emc::display_user_settings::load()
{
  return emc::display_user_settings();
}

void emc::display_user_settings::clear()
{

}

void emc::display_user_settings::set_display_brightness(unsigned int display, float brightness)
{

}

float emc::display_user_settings::get_display_brightness(unsigned int display) const
{
  return 0;
}

emc::display_user_settings::display_user_settings()
{

}
