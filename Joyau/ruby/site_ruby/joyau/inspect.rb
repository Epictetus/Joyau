#
# inspect.rb - Inspect method for Joyau's classes
#

# :stopdoc:

#
# Copyright 2009 Verhetsel Kilian
# This program can be distributed under the terms of the GNU LGPL.
# See the file COPYING.
#

# :startdoc:

module Joyau
  class Sprite
    def inspect # :nodoc:
      "#{self.class}(#{self.picture})"
    end
  end

  class Vector3f
    def inspect # :nodoc:
      "#{self.class}(#{self.x}, #{self.y}, #{self.z})"
    end
  end

  class Point
    def inspect # :nodoc:
      "#{self.class}(#{self.x}, #{self.y})"
    end
  end

  class Rect
    def inspect # :nodoc:
      "#{self.class}(#{self.x}, #{self.y}, w: #{self.w}, h: #{self.h})"
    end
  end

  class Drawable
    def inspect # :nodoc:
      "#{self.class}(#{self.x}, #{self.y}, w: #{self.w}, h: #{self.h})"
    end
  end

  class DrawableText
    def inspect # :nodoc:
      "#{self.class}(#{self.text})"
    end
  end

  class IntraText
    def inspect # :nodoc:
      "#{self.class}(#{self.text})"
    end
  end

  class CollisionType
    def inspect # :nodoc:
      "#{self.class}(left:#{left}, right:#{right}, up:#{up}, down:#{down}" + 
        ", content:#{content})"
    end
  end

  class Tile
    def inspect # :nodoc:
      "#{self.class}(#{tileset}, #{tileX}, #{tileY}, #{x}, #{y})"
    end
  end
end
