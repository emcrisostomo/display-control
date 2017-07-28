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

#include "display_control_config.h"
#include "display-control.h"
#include "gettext.h"
#ifdef HAVE_GETOPT_LONG
#  include <getopt.h>
#endif
#include <iostream>
#include <string>
#include <vector>
#include <IOKit/graphics/IOGraphicsLib.h>
#include <ApplicationServices/ApplicationServices.h>

#define _(String) gettext(String)

static bool lflag = false;
static bool mflag = false;
static bool vflag = false;
static std::vector<int> displays;

static void print_version(std::ostream& stream);
static void usage(std::ostream& stream);

static void parse_opts(int argc, char *const argv[])
{
  int ch;
  std::string short_options = "d:lmv";

#ifdef HAVE_GETOPT_LONG
  int option_index = 0;
  static struct option long_options[] = {
    {"display", required_argument, nullptr, 'd'},
    {"help",    no_argument,       nullptr, 'h'},
    {"list",    no_argument,       nullptr, 'l'},
    {"verbose", no_argument,       nullptr, 'v'},
    {"version", no_argument,       nullptr, DC_OPT_VERSION},
    {nullptr, 0,                   nullptr, 0}
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
      break;

    case 'm':
      mflag = true;
      break;

    case 'v':
      vflag = true;
      break;
      break;

    case DC_OPT_VERSION:
      print_version(std::cout);
      exit(DC_EXIT_OK);

    case '?':
      usage(std::cerr);
      exit(DC_EXIT_UNKNOWN_OPT);
    }
  }
}

static void print_version(std::ostream& stream)
{
  stream << PACKAGE_STRING << "\n";
  stream <<
         "Copyright (C) 2017 Enrico M. Crisostomo <enrico.m.crisostomo@gmail.com>.\n";
  stream <<
         _("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
  stream <<
         _("This is free software: you are free to change and redistribute it.\n");
  stream << _("There is NO WARRANTY, to the extent permitted by law.\n");
  stream << "\n";
  stream << _("Written by Enrico M. Crisostomo.");
  stream << std::endl;
}

static void usage(std::ostream& stream)
{
#ifdef HAVE_GETOPT_LONG
  stream << PACKAGE_STRING << "\n\n";
  stream << _("Usage:\n");
  stream << PACKAGE_NAME << _(" [OPTION] ... path ...\n");
  stream << "\n";
  stream << _("Options:\n");
  stream << " -d, --display=n       " << _("Choose display n.\n");
  stream << " -h, --help            " << _("Show this message.\n");
  stream << " -l, --list            " << _("List displays.\n");
  stream << " -v, --verbose         " << _("Print verbose output.\n");
  stream << "     --version         " << _("Print the version of ") << PACKAGE_NAME << _(" and exit.\n");
  stream << "\n";
#else
  string option_string = "[01adeEfhilLMmnortuvx]";

  stream << PACKAGE_STRING << "\n\n";
  stream << _("Usage:\n");
  stream << PACKAGE_NAME << " " << option_string << " path ...\n";
  stream << "\n";
  stream << _("Options:\n");
  stream << " -d  Watch directories only.\n";
  stream << " -h  Show this message.\n";
  stream << " -l  List displays.\n";
  stream << " -v  Print verbose output.\n";
  stream << "\n";
#endif

  stream << _("\nSee the man page for more information.\n\n");

  stream << _("Report bugs to <") << PACKAGE_BUGREPORT << ">.\n";
  stream << PACKAGE << _(" home page: <") << PACKAGE_URL << ">.";
  stream << std::endl;
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

}