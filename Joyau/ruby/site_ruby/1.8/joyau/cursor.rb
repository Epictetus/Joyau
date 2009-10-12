#
# cursor.rb
# Copyright 2009 Verhetsel Kilian
#
# This program can be distributed under the terms of the GNU GPL.
# See the file COPYING.
#

#
# Joyau has got a Cursor class, which allows to move a sprite
# via the stick, but this class isn't a module which can be included in
# any drawable. Therefore, StickFollower allows to do that, and works
# pretty much the same way.
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
    Pad.update
    
    unless @sensibility == 0
      move(Pad.stickX / @sensibility, Pad.stickY / @sensibility);

      x_pos = self.x > @area.x + @area.w ? @area.x + @area.w : self.x
      x_pos = self.x < @area.x ? @area.x : self.x

      y_pos = self.y > @area.y + @area.h ? @area.y + @area.h : self.y
      y_pos = self.y < @area.y ? @area.y : self.y

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
