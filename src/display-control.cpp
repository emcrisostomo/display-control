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

#ifdef HAVE_CONFIG_H
#  include "display_control_config.h"
#endif
#ifdef HAVE_CMAKE_CONFIG_H
#  include "cmake_config.h"
#endif
#include "display-control.h"
#include "gettext_defs.h"
#ifdef HAVE_GETOPT_LONG
#  include <getopt.h>
#endif
#include <iostream>
#include <string>
#include <vector>
#include "display.h"
#include "display_user_settings.h"

#define _(String) gettext(String)
static void print_version(std::ostream& stream);
static void usage(std::ostream& stream);
static bool check_command();
static int list_displays();
static int restore_displays();
static int set_brightness();

static unsigned int command_counter = 0;
static bool bflag = false;
static bool lflag = false;
static bool rflag = false;
static bool vflag = false;
static std::vector<unsigned int> displays;
static float brightness = 0.0;

static void parse_opts(int argc, char *const argv[])
{
  int ch;
  std::string short_options = "b:d:hlrv";

#ifdef HAVE_GETOPT_LONG
  int option_index = 0;
  static struct option long_options[] = {
    {"brightness", required_argument, nullptr, 'b'},
    {"display",    required_argument, nullptr, 'd'},
    {"help",       no_argument,       nullptr, 'h'},
    {"list",       no_argument,       nullptr, 'l'},
    {"restore",    no_argument,       nullptr, 'r'},
    {"verbose",    no_argument,       nullptr, 'v'},
    {"version",    no_argument,       nullptr, DC_OPT_VERSION},
    {nullptr, 0,                      nullptr, 0}
  };

  while ((ch = getopt_long(argc,
                           argv,
                           short_options.c_str(),
                           long_options,
                           &option_index)) != -1)
  {
#else
    while ((ch = getopt(argc, argv, short_options.c_str())) != -1)
    {
#endif
    switch (ch)
    {
    case 'b':
      try
      {
        brightness = std::stof(optarg, nullptr);
      }
      catch (std::invalid_argument& ex)
      {
        std::cerr << _("Invalid brightness value: ") << optarg << "\n";
        exit(DC_EXIT_ILLEGAL_VALUE);
      }

      if (brightness < 0 || brightness > 1)
      {
        std::cerr << _("Invalid brightness value: ") << optarg << "\n";
        exit(DC_EXIT_ILLEGAL_VALUE);
      }

      bflag = true;
      ++command_counter;
      break;

    case 'd':
      int display_num;

      try
      {
        display_num = std::stoi(optarg, nullptr, 10);
      }
      catch (std::invalid_argument& ex)
      {
        std::cerr << _("Invalid display number: ") << optarg << "\n";
        exit(DC_EXIT_ILLEGAL_VALUE);
      }

      if (display_num < 0)
      {
        std::cerr << _("Invalid display number: ") << optarg << "\n";
        exit(DC_EXIT_ILLEGAL_VALUE);
      }

      displays.push_back(display_num);
      break;

    case 'h':
      usage(std::cout);
      exit(DC_EXIT_OK);

    case 'l':
      lflag = true;
      ++command_counter;
      break;

    case 'r':
      rflag = true;
      ++command_counter;
      break;

    case 'v':
      vflag = true;
      break;

    case DC_OPT_VERSION:
      print_version(std::cout);
      exit(DC_EXIT_OK);

    case '?':
      exit(DC_EXIT_UNKNOWN_OPT);
    }
  }

  if (!check_command())
  {
    exit(DC_EXIT_ILLEGAL_COMMAND);
  }

  // validate options
  if (optind != argc)
  {
    std::cerr << _("Invalid number of arguments.") << "\n";
    exit(DC_EXIT_UNKNOWN_OPT);
  }

  if (displays.empty()) displays.push_back(0);
}

static bool check_command()
{
  switch (command_counter)
  {
  case 0:
    lflag = true;

  case 1:
    return true;

  default:
    std::cerr << _("Only one command can be specified.") << "\n";
    return false;
  }
}

static void print_version(std::ostream& stream)
{
  //@formatter:off
  stream << PACKAGE_STRING << "\n";
  stream << "Copyright (C) 2017 Enrico M. Crisostomo <enrico.m.crisostomo@gmail.com>.\n";
  stream << _("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
  stream << _("This is free software: you are free to change and redistribute it.\n");
  stream << _("There is NO WARRANTY, to the extent permitted by law.\n");
  stream << "\n";
  stream << _("Written by Enrico M. Crisostomo.");
  stream << std::endl;
  //@formatter:off
}

static void usage(std::ostream& stream)
{
  //@formatter:off
#ifdef HAVE_GETOPT_LONG
  stream << PACKAGE_STRING << "\n\n";
  stream << _("Usage:\n");
  stream << PACKAGE_NAME << _(" [OPTION] ... path ...\n");
  stream << "\n";
  stream << _("Options:\n");
  stream << " -b, --brightness=b    " << _("Set brightness to b.\n");
  stream << " -d, --display=n       " << _("Choose display n.\n");
  stream << " -h, --help            " << _("Show this message.\n");
  stream << " -l, --list            " << _("List displays.\n");
  stream << " -r, --restore         " << _("Restore display brightness.\n");
  stream << " -v, --verbose         " << _("Print verbose output.\n");
  stream << "     --version         " << _("Print the version of ") << PACKAGE_NAME << _(" and exit.\n");
  stream << "\n";
#else
  std::string option_string = "[bdhlv]";

  stream << PACKAGE_STRING << "\n\n";
  stream << _("Usage:\n");
  stream << PACKAGE_NAME << " " << option_string << " path ...\n";
  stream << "\n";
  stream << _("Options:\n");
  stream << " -b  Set brightness to b.\n";
  stream << " -d  Choose display d.\n";
  stream << " -h  Show this message.\n";
  stream << " -l  List displays.\n";
  stream << " -v  Print verbose output.\n";
  stream << "\n";
#endif

  stream << _("\nSee the man page for more information.\n\n");

  stream << _("Report bugs to <") << PACKAGE_BUGREPORT << ">.\n";
  stream << PACKAGE << _(" home page: <") << PACKAGE_URL << ">.";
  stream << std::endl;
  //@formatter:on
}

int main(int argc, char *const argv[])
{
  // Trigger gettext operations
#ifdef ENABLE_NLS
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);
#endif

  parse_opts(argc, argv);

  try
  {
    if (bflag) return set_brightness();
    if (lflag) return list_displays();
    if (rflag) return restore_displays();
  }
  catch (std::exception& ex)
  {
    std::cerr << _("An error occurred: ") << ex.what() << "\n";
  }

  std::cerr << _("No operation was performed.\n This is probably a bug.\n");
  return DC_EXIT_ILLEGAL_COMMAND;
}

int set_brightness()
{
  std::vector<emc::display> active_displays = emc::display::find_active();

  int ret = DC_EXIT_OK;

  for (auto& d : displays)
  {
    if (d >= active_displays.size())
    {
      std::cerr << _("Invalid display ") << d << "\n";
      ret = DC_EXIT_UNKNOWN_DISPLAY;
      continue;
    }

    try
    {
      active_displays[d].set_brightness(brightness);
    }
    catch (std::runtime_error& ex)
    {
      std::cerr << _("An error occurred: ") << ex.what() << "\n";
    }
  }

  return ret;
}

int list_displays()
{
  std::vector<emc::display> active_displays = emc::display::find_active();

  int ret = DC_EXIT_OK;

  for (auto& d : displays)
  {
    if (d >= active_displays.size())
    {
      std::cerr << _("Invalid display ") << d << "\n";
      ret = DC_EXIT_UNKNOWN_DISPLAY;
      continue;
    }

    try
    {
      float current_brightness = active_displays[d].get_brightness();

      std::cout << d << ":" << current_brightness << "\n";
    }
    catch (std::runtime_error& ex)
    {
      std::cout << d << ":" << _("n/a") << "\n";
    }
  }

  return ret;
}

int restore_displays()
{
  std::vector<emc::display> active_displays = emc::display::find_active();
  emc::display_user_settings settings = emc::display_user_settings::load();

  int ret = DC_EXIT_OK;

  for (auto& d : displays)
  {
    if (d >= active_displays.size())
    {
      std::cerr << _("Invalid display ") << d << "\n";
      ret = DC_EXIT_UNKNOWN_DISPLAY;
      continue;
    }

    if (!settings.has_configuration(d))
    {
      std::cerr << _("Display has no configuration: ") << d << "\n";
      continue;
    }

    active_displays[d].set_brightness(settings.get_display_brightness(d));
  }

  return ret;
}
