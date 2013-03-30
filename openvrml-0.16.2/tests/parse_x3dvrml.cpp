// -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; -*-
//
// Copyright 2004, 2005  Braden McDaniel
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

# include <iostream>
# include "test_browser.h"

using namespace std;
using namespace openvrml;

int main()
{
    try {
        test_browser b;
        b.create_vrml_from_stream(cin, x3d_vrml_media_type);
    } catch (invalid_vrml & ex) {
        cerr << ex.url << ':' << ex.line << ':' << ex.column << ": error: "
             << ex.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}