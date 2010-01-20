# enumerable.rb - Includes Enumerable in some of Joyau's classes

# :stopdoc:

#
# Copyright 2009-2010 Verhetsel Kilian
# This program can be distributed under the terms of the GNU LGPL.
# See the file COPYING.
#

# :startdoc:

module Joyau
  class Buffer
    include Enumerable
    
    def each
      (0...w).each do |x|
        (0...h).each do |y|
          yield self[x, y].to_col
        end
      end
    end

    # Iterates through a Buffer, yielding a point's position, and its color.
    #  buffer.each_with_pos { |x, y, col| ... }
    def each_with_pos
      (0...w).each do |x|
        (0...h).each do |y|
          yield x, y, self[x, y].to_col
        end
      end
    end
  end

  class GameMap
    include Enumerable

    def each
      # A map is a collection of tiles, rather than a collection of tilesets.
      each_tiles do |tile|
        yield tile
      end
    end
  end
end
