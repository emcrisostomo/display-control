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

#ifndef DISPLAY_CONTROL_SERVICE_GUARD_H
#define DISPLAY_CONTROL_SERVICE_GUARD_H

#include <IOKit/graphics/IOGraphicsLib.h>

namespace emc
{
  class service_guard
  {
  private:
    io_service_t handle;

  public:
    service_guard() : handle(0)
    {
    }

    explicit service_guard(io_service_t handle) : handle(handle)
    {
    }

    service_guard(const service_guard&) = delete;
    service_guard& operator=(const service_guard&) = delete;

    service_guard(service_guard&& other) noexcept
    {
      handle = other.handle;
      other.handle = 0;
    }

    service_guard& operator=(service_guard&& other) noexcept
    {
      if (this == &other) return *this;

      if (handle) IOObjectRelease(handle);
      handle = other.handle;
      other.handle = 0;

      return *this;
    }

    virtual ~service_guard()
    {
      if (handle) IOObjectRelease(handle);
    }

    operator io_service_t() const
    {
      return handle;
    }

    bool operator ==(const io_service_t &value) const
    {
      return handle == value;
    }

    bool operator !=(const io_service_t &value) const
    {
      return !(*this == value);
    }
  };
}
#endif //DISPLAY_CONTROL_SERVICE_GUARD_H
