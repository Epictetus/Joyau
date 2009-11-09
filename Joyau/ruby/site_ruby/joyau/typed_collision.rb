#
# typed_collision.rb - a mixin in order to use CollisionType
#

# :stopdoc:

#
# Copyright 2009 Verhetsel Kilian
# This program can be distributed under the terms of the GNU LGPL.
# See the file COPYING.
#

# :startdoc:

module Joyau
  # This mixin allows to use Joyau's CollisionType class in yours.
  module TypedCollision
    def collide(obj)
      if @collision_type.content
        DrawableRect rect(x, y, w, h)
        return rect.collide(obj)
      end

      line = Line.new
      if @collision_type.right
        line.setPos(x + w, y)
        line.setPoint(x + w, y + h)
        return true if line.collide(obj)
      end

      if @collision_type.left
        line.setPos(x, y)
        line.setPoint(x, y + h)
        return true if line.collide(obj)
      end

      if @collision_type.up
        line.setPos(x, y)
        line.setPoint(x + w, y)
        return true if line.collide(obj)
      end
       
      if @collision_type.down
        line.setPos(x, y + h)
        line.setPoint(x + w, y + h)
        return true if line.collide(obj)
      end

      return false
    end

    attr_writer :collision_type
  end
end
