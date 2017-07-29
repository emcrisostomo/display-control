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
#include "object-guard.h"
#include <IOKit/graphics/IOGraphicsLib.h>

namespace emc
{
  static io_service_t find_io_service(CGDirectDisplayID display_id);
  static bool compare(CFNumberRef number, uint32_t uint32);

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
    //@formatter:off
    active = static_cast<bool>(CGDisplayIsActive(this->display_id));
    asleep = static_cast<bool>(CGDisplayIsAsleep(this->display_id));
    builtin = static_cast<bool>(CGDisplayIsBuiltin(this->display_id));
    in_mirror_set = static_cast<bool>(CGDisplayIsInMirrorSet(this->display_id));
    main = static_cast<bool>(CGDisplayIsMain(this->display_id));
    online = static_cast<bool>(CGDisplayIsOnline(this->display_id));
    stereo = static_cast<bool>(CGDisplayIsStereo(this->display_id));
    mirrored_display = CGDisplayMirrorsDisplay(this->display_id);
    opengl_used = static_cast<bool>(CGDisplayUsesOpenGLAcceleration(this->display_id));

    mirrored = (mirrored_display == kCGNullDirectDisplay);
    //@formatter:on
  }

  bool display::is_active() const
  {
    return active;
  }

  bool display::is_asleep() const
  {
    return asleep;
  }

  bool display::is_builtin() const
  {
    return builtin;
  }

  bool display::is_in_mirror_set() const
  {
    return in_mirror_set;
  }

  bool display::is_main() const
  {
    return main;
  }

  bool display::is_online() const
  {
    return online;
  }

  bool display::is_stereo() const
  {
    return stereo;
  }

  bool display::is_opengl_used() const
  {
    return opengl_used;
  }

  bool display::is_mirrored() const
  {
    return mirrored;
  }

  float display::get_brightness() const
  {
    io_service_t service = find_io_service(this->display_id);
    float current_brightness;

    IOReturn ret = IODisplayGetFloatParameter(service,
                                              kNilOptions,
                                              CFSTR(kIODisplayBrightnessKey),
                                              &current_brightness);

    if (ret != kIOReturnSuccess)
    {
      std::ostringstream oss;
      oss << _("IODisplayGetFloatParameter returned error: ") << ret;

      throw std::runtime_error(oss.str());
    }

    return current_brightness;
  }

  void display::set_brightness(float brightness)
  {
    io_service_t service = find_io_service(this->display_id);

    IOReturn ret = IODisplaySetFloatParameter(service,
                                              kNilOptions,
                                              CFSTR(kIODisplayBrightnessKey),
                                              brightness);

    if (ret != kIOReturnSuccess)
    {
      std::ostringstream oss;
      oss << _("IODisplaySetFloatParameter returned error: ") << ret;

      throw std::runtime_error(oss.str());
    }
  }

  io_service_t find_io_service(CGDirectDisplayID display_id)
  {
    uint32_t vendor = CGDisplayVendorNumber(display_id);
    uint32_t model = CGDisplayModelNumber(display_id);
    uint32_t serial = CGDisplaySerialNumber(display_id);

    io_iterator_t service_iterator;

    if (IOServiceGetMatchingServices(kIOMasterPortDefault,
                                     IOServiceMatching("IODisplayConnect"),
                                     &service_iterator) != kIOReturnSuccess)
      return 0;

    emc::object_guard<io_iterator_t, decltype(IOObjectRelease)> service_guard(service_iterator, IOObjectRelease);

    io_service_t service;

    while ((service = IOIteratorNext(service_iterator)) != 0)
    {
      CFDictionaryRef info = IODisplayCreateInfoDictionary(service, kIODisplayNoProductName);
      emc::object_guard<CFDictionaryRef, decltype(CFRelease)> info_o_guard(info, CFRelease);

      auto vendorID = static_cast<CFNumberRef>(CFDictionaryGetValue(info, CFSTR(kDisplayVendorID)));
      auto productID = static_cast<CFNumberRef>(CFDictionaryGetValue(info, CFSTR(kDisplayProductID)));
      auto serialNumber = static_cast<CFNumberRef>(CFDictionaryGetValue(info, CFSTR(kDisplaySerialNumber)));

      if (compare(vendorID, vendor) &&
          compare(productID, model) &&
          compare(serialNumber, serial))
      {
        return service;
      }
    }

    return 0;
  }

  bool compare(CFNumberRef number, uint32_t uint32)
  {
    if (number == nullptr) return (uint32 == 0);

    int64_t number_value;

    return CFNumberGetValue(number, kCFNumberSInt64Type, &number_value) &&
           number_value == uint32;

  }
}