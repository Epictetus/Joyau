#
# Pad is a module which allows to read the keys. It's reopened in joyau/pad.rb,
# in order to add some features, like method which allow to know how many
# keys are held, ...
#
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
