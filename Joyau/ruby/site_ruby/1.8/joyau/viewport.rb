#
# viewport.rb
# Copyright 2009 Verhetsel Kilian
#
# This program can be distributed under the terms of the GNU GPL.
# See the file COPYING.
#

#
# When writing  game, we might have to think about hero's position,
# obstacle's position, ... but this can be confusing.
#
# When a viewport is drawed, it's object are moved, drawed, and then, their
# moves are cancelled. So, you can think about the position in the world without
# considering the PSP's screen.
#
class Viewport < Drawable
  attr_reader :objects

  #
  # Creates a new Viewport.
  #
  def self.new
    ret = super
    ret.initialize

    return ret
  end

  def initialize
    @objects = []
  end

  #
  # Inserts a new object in the Viewport. This method raises an exception
  # if the given object is not a Drawable.
  #
  def <<(obj)
    raise TypeError, "obj has to be a Drawable." unless obj.is_a? Drawable
    @objects << obj
  end

  #
  # Draws all the objects in the Viewport.
  #
  def draw
    @objects.each { |i|
      old_pos = Point.new(i.x, i.y)
      i.pos = Point.new(x + i.x, y + i.y)
      i.draw
      i.pos = old_pos
    }
  end

  public :initialize
end
