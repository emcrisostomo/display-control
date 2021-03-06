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

#include "filesystem.h"
#include "gettext_defs.h"
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>

static void mkdir_or_throw(const std::string& path);

void mkdir_or_throw(const std::string& path)
{
  if (mkdir(path.c_str(), S_IRWXU) != 0 && (errno != EISDIR && errno != EEXIST))
  {
    throw std::runtime_error(std::strerror(errno));
  }
}

void emc::filesystem::create_dir(const std::string& path)
{
  for (std::string::const_iterator it = path.begin(); it < path.end(); it++)
  {
    if (*it == '/')
    {
      mkdir_or_throw(std::string(path.begin(), it + 1));
    }
  }

  mkdir_or_throw(path);
}

bool emc::filesystem::is_dir(const std::string& path)
{
  struct stat info{};

  int ret = stat(path.c_str(), &info);

  if (ret == 0)
  {
    return (info.st_mode & S_IFDIR) != 0;
  }

  switch (errno)
  {
  case ENOENT:
    return false;

  default:
    throw std::runtime_error(std::strerror(errno));
  }
}
