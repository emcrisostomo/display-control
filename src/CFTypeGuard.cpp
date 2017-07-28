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

#include "CFTypeGuard.h"

namespace emc
{
  bool CFTypeGuard::is_valid(const CFTypeRef& handle)
  {
    return (handle != nullptr);
  }

  CFTypeGuard::CFTypeGuard() : h(nullptr)
  {}

  CFTypeGuard::CFTypeGuard(CFTypeRef handle) : h(handle)
  {}

  CFTypeGuard::~CFTypeGuard()
  {
    if (is_valid())
    {
      CFRelease(h);
    }
  }

  CFTypeGuard::operator CFTypeRef() const
  {
    return h;
  }

  bool CFTypeGuard::is_valid() const
  {
    return CFTypeGuard::is_valid(h);
  }

  CFTypeGuard& CFTypeGuard::operator=(const CFTypeRef& handle)
  {
    if (is_valid() && h != handle) CFRelease(h);

    h = handle;

    return *this;
  }

  CFTypeGuard::CFTypeGuard(CFTypeGuard&& other) noexcept
  {
    h = other.h;
    other.h = nullptr;
  }

  CFTypeGuard& CFTypeGuard::operator=(CFTypeGuard&& other) noexcept
  {
    if (this == &other) return *this;

    if (is_valid() && h != other.h) CFRelease(h);

    h = other.h;
    other.h = nullptr;

    return *this;
  }
}