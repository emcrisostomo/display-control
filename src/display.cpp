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

#include "display.h"
#include <system_error>
#include <sstream>
#include "gettext_defs.h"

namespace emc
{
  std::vector<display> display::find_active()
  {
    std::vector<display> active_displays;

    CGDirectDisplayID display_ids[DC_MAX_DISPLAYS];
    CGDisplayCount display_count;
    CGDisplayErr err = CGGetOnlineDisplayList(DC_MAX_DISPLAYS,
                                              display_ids,
                                              &display_count);
    if (err != CGDisplayNoErr)
    {
      std::ostringstream oss;
      oss << _("CGGetOnlineDisplayList returned error: ");
      oss << err;

      throw std::runtime_error(oss.str());
    }

    for (CGDisplayCount i = 0; i < display_count; ++i)
    {
      CGDirectDisplayID display_id = display_ids[i];
      CGDisplayModeRef mode = CGDisplayCopyDisplayMode(display_id);

      if (mode == nullptr)
        continue;

      CGDisplayModeRelease(mode);

      active_displays.push_back(emc::display(display_id));
    }

    return active_displays;
  }

  display::display(CGDirectDisplayID display_id) : display_id(display_id)
  {
  }
}