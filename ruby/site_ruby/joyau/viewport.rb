#
# viewport.rb - a Viewport class for Joyau, in order to use relative position.
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
  # When writing  game, we might have to think about hero's position,
  # obstacle's position, ... but this can be confusing.
  #
  # When a viewport is drawed, it's object are moved, drawed, and then, their
  # moves are cancelled. So, you can think about the position in the world without
  # considering the PSP's screen.
  #
  class Viewport < Drawable
    attr_reader :objects

    # Creates a new Viewport.
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

      return self
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
        i.clearMove
      }
    end

    #
    # Converts an aboslute position (given as a point) to a relative position.
    #
    def abs2rel(point)
      return Point.new(point.x - self.x, point.y - self.y)
    end

    #
    # Converts a relative position (given as a point) to an absolute position.
    #
    def rel2abs(point)
      return Point.new(point.x + self.x, point.y + self.y)
    end

    #
    # Centers the view on point, which is relative to the view.
    #
    def center_on(point)
      p = rel2abs(point)
      move(240 - p.x, 136 - p.y)
    end
  end
end
