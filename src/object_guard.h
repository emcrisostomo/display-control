/*
 * Copyright (c) 2017-2019 Enrico M. Crisostomo
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

#ifndef DISPLAY_CONTROL_OBJECTGUARD_H
#define DISPLAY_CONTROL_OBJECTGUARD_H

namespace emc
{
  template<typename T, typename F, T null_handle = nullptr>
  class object_guard
  {
  public:
    object_guard(T handle, F deleter) : handle(handle),
                                        deleter(deleter)
    {
    }

    object_guard(const object_guard&) = delete;
    object_guard& operator=(const object_guard&) = delete;

    object_guard(object_guard&& other) noexcept
    {
      handle = other.handle;
      other.handle = null_handle;
    }

    object_guard& operator=(object_guard&& other) noexcept
    {
      if (this == &other) return *this;

      if (handle != null_handle) deleter(handle);
      handle = other.handle;
      other.handle = null_handle;

      return *this;
    }

    virtual ~object_guard()
    {
      if (handle != null_handle)
        deleter(handle);
    }

    operator T() const
    {
      return handle;
    }

    bool operator==(const T& value) const
    {
      return handle == value;
    }

    bool operator!=(const T& value) const
    {
      return !(*this == value);
    }

  private:
    T handle;
    F deleter;
  };
}

#endif //DISPLAY_CONTROL_OBJECTGUARD_H
