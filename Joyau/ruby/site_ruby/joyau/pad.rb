#
# pad.rb - Modification to the pad module in Ruby
#

# :stopdoc:

#
# Copyright 2009 Verhetsel Kilian
# This program can be distributed under the terms of the GNU LGPL.
# See the file COPYING.
#

# :startdoc:
module Joyau
  module Pad

    #
    # All the Keys stored in an array.
    #
    KEYS = [
            SELECT, START, L, R, CROSS, SQUARE, TRIANGLE, CIRCLE, UP, DOWN, LEFT,
            RIGHT, HOLD
           ]

    #
    # Returns an array containing all the keys, excepted those who
    # aren't held.
    #
    def self.held_keys
      KEYS.reject { |i| !held?(i) }
    end

    #
    # Returns an array containing all the keys, excepted those who
    # aren't pressed.
    #
    def self.pressed_keys
      KEYS.reject { |i| !pressed?(i) }
    end

    #
    # Returns an array containing all the keys, excepted those who
    # aren't released.
    #
    def self.released_keys
      KEYS.reject { |i| !released?(i) }
    end

    #
    # Returns how many keys are held.
    #
    def self.held_nbr
      held_keys.size
    end

    #
    # Returns how many keys are pressed.
    #
    def self.pressed_nbr
      pressed_keys.size
    end

    #
    # Returns how many keys are released.
    #
    def self.released_nbr
      released_keys.size
    end
  end
end
