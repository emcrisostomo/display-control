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

#ifndef DISPLAY_CONTROL_CFTYPEGUARD_H
#define DISPLAY_CONTROL_CFTYPEGUARD_H

#include <CoreFoundation/CoreFoundation.h>

namespace emc
{
  class CFTypeGuard
  {
  public:
    static bool is_valid(const CFTypeRef& handle);

    CFTypeGuard();
    CFTypeGuard(CFTypeRef handle);
    CFTypeGuard(const CFTypeGuard&) = delete;
    CFTypeGuard(CFTypeGuard&& other) noexcept;
    virtual ~CFTypeGuard();
    operator CFTypeRef() const;
    bool is_valid() const;
    CFTypeGuard& operator=(const CFTypeGuard&) = delete;
    CFTypeGuard& operator=(const CFTypeRef& handle);
    CFTypeGuard& operator=(CFTypeGuard&& other) noexcept;
  private:
    CFTypeRef h;
  };
}

#endif //DISPLAY_CONTROL_CFTYPEGUARD_H
