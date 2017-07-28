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

#ifndef DISPLAY_CONTROL_OBJECTGUARD_H
#define DISPLAY_CONTROL_OBJECTGUARD_H

namespace emc
{
  template<typename T, typename F>
  class object_guard
  {
  public:
    object_guard(T handle, F resource_releaser) : handle(handle), resource_releaser(resource_releaser)
    {
    }

    virtual ~object_guard()
    {
      resource_releaser(handle);
    }

  private:
    T handle;
    F *resource_releaser;
  };
}

#endif //DISPLAY_CONTROL_OBJECTGUARD_H
