
iOS iAd wrapper for Marmalade Quick
===================================

This is a 1:1 wrapper for the s3eIOSIAd C extension so that you
can use it from Marmalade Quick Lua code. Functionality and usage is the
same as the C++ version.

It's supported on iOS only.

Notes:

- All the paths here are relative to the root folder of your installed
  Marmalade SDK.

- I've started just naming these Quick wrapper projects Qxxxx rather than
  MarmaladeQuickXXX - less typing and easier to include as subprojects if
  folder and mkf have the same name :)


Prerequisites
-------------

1. Marmalade SDK 7.7 or newer for Quick tooling upgrades (in 7.7)

2. Put this project on Marmalade search paths:

  Easiest option is to keep the project in you main github folder. Then, if you
  haven't already, add your github root to the global search by putting the
  following in < Marmalade >/s3e/s3e-default.mkf:

        options { module_path="path/to/my/github/projects/root" }

  Multiple Marmalade installs can pick up your project then.
  
  If you dont have a main github folder, you can add just this project:
  
        options { module_path="path/to/QIosIAd" }


Setup: Add and build this wrapper into Quick
--------------------------------------------

1. Edit quick/quickuser_tolua.pkg and add this new line:

        $cfile "path/to/QIosIAd/QIosIAd.h"

2. Edit quick/quickuser.mkf and add QIosIAd to the 'subprojects' block:

        subprojects
        {
            QIosIAd
        }
   
3. Run quick/quickuser_tolua.bat to generate Lua bindings.

4. Rebuild the Quick binaries by running the script(s):
   quick/build_quick_prebuilt.bat etc.

   
Using IAd in your app
--------------------------------------

1. Add the C++ extension to your app's .mkb file:

        subproject s3eIOSIAd

   This bundles platform specific extension libraries in your app
   at deploy time. Forgetting this means all calls will quietly fail!

2. Use the Lua APIs in your app!
   
   The Lua functions match the C++ ones in s3eIOSIAd.h
   
   Make sure you use iosIAd.xxx() and not iosIAd:xxx()!

   
## Events

Quick events are provided to match the C callbacks from s3eIOSIAd.
Quick IAd uses a single event, also called "iosIAd", which is registered
with the usual system:addEventListener function. Iad events just tell you
if an action occurred - there is no additional info. The "action" value
of the table passed to your listener indicates which callbacks/action occurred.
Actions are:

- "bannerLoaded"
- "failed" (an error occurred, call getError() to find out what)
- "adStarting"
- "adFinished"


Example:
        
        function iadListener(event)
            if event.action == "bannerLoaded" then
                dbg.print("banner was loaded")
            elseif event.action == "failed" then
                dbg.print("an error occurred: " .. iosIAd.getError())
            --etc
            end
        end

        if iosIAd.isAvailable() then
            if iosIAd.start() then
                system:addEventListener("iosIAd", iadListener)
            end
        end

------------------------------------------------------------------------------------------
(C) 2015 Nick Smith.

All code is provided under the MIT license unless stated otherwise:

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
