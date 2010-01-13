#
# cursor.rb - a Cursor module for Joyau, for object which move with the stick.
#

# :stopdoc:

#
# Copyright 2009-2010 Verhetsel Kilian
# This program can be distributed under the terms of the GNU LGPL.
# See the file COPYING.
#

# :startdoc:
module Joyau
  #
  # Joyau has got a Cursor class, which allows to move a sprite
  # via the stick, but this class isn't a module which can be included in
  # any drawable. Therefore, StickFollower allows to do that, and works
  # pretty much the same way.
  #
  #  
  #
  module StickFollower
    #
    # If the sensibility is higher, the object moves harder.
    #
    attr_accessor :sensibility

    #
    # The area in which the object may move.
    #
    attr_accessor :area

    #
    # Updates the object's position, according to the analogic stick's position.
    #
    def update_pos
      unless @sensibility == 0
        move(Pad.stickX / @sensibility, Pad.stickY / @sensibility);

        if self.x > (@area.x + @area.w)
          x_pos = @area.x + @area.w
        elsif self.x < @area.x
          x_pos = @area.x
        else
          x_pos = self.x
        end
        
        if self.y > (@area.y + @area.h)
          y_pos = @area.y + @area.h
        elsif self.y < @area.y
          y_pos = @area.y
        else
          y_pos = self.y
        end

        setPos(x_pos, y_pos)
      end
    end

    #
    # Configurates the cursor's attribute
    #
    def cursor_conf(sensibility, area = Rect.new(0, 0, 480, 272))
      @sensibility = sensibility
      @area = area
    end
  end
end
