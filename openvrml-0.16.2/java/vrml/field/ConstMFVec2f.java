//
// OpenVRML
//
// Copyright (C) 2001  Henri Manson 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// 

package vrml.field;

import vrml.ConstMField;

/**
 * Represents a read-only VRML MFVec2f field in Java.
 */
public class ConstMFVec2f extends ConstMField
{
  public native int getSize();

  /**
   * Construct a new MFVec2f field in OpenVRML using the given params.
   *
   * @param vec2s An array of sets of x,y values
   */
  private native void CreateObject(float vec2s[][]);

  /**
   * Construct a new MFVec2f field in OpenVRML using the given params.
   *
   * @param size Number of SFVec2f elements passed in.
   * @param vec2s List of x,y pairs
   */
  private native void CreateObject(int size, float vec2s[]);

  /**
   * Construct a read-only MFVec2f field.
   *
   * @param vec2s An array of sets of x,y values
   */
  public ConstMFVec2f(float vec2s[][])
  {
    CreateObject(vec2s);
  }

  /**
   * Construct a read-only MFVec2f field.
   *
   * @param vec2s List of x,y pairs
   */
  public ConstMFVec2f(float vec2s[])
  {
    CreateObject(vec2s.length / 2, vec2s);
  }

  /**
   * Construct a read-only MFVec2f field.
   *
   * @param size Number of SFVec2f elements passed in.
   * @param vec2s List of x,y pairs
   */
  public ConstMFVec2f(int size, float vec2s[])
  {
    CreateObject(size, vec2s);
  }

  /**
   * Retrieves the value of an MFVec2f field.
   *
   * @param vec2s 2D array of x,y pairs to be returned.
   */
  public native void getValue(float vec2s[][]);

  /**
   * Retrieves the value of an MFVec2f field.
   *
   * @param vec2s Array of x,y pairs to be returned.
   */
  public native void getValue(float vec2s[]);

  /**
   * Retrieves a specific element in an MFVec2f and
   * returns it as a float array.
   *
   * @param index Position of desired element
   * @param vec2s Element at specified position
   */
  public native void get1Value(int index, float vec2s[]);

  /**
   * Retrieves a specific element in an MFVec2f and
   * returns it as an SFVec2f.
   *
   * @param index Position of desired element
   * @param vec Element at specified position
   */
  public native void get1Value(int index, SFVec2f vec);

  public native String toString();
}
