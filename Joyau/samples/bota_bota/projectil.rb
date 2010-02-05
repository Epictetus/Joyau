require 'joyau/inherited'

class Projectil < Joyau::Sprite
  joyau_inherited

  def initialize(pic = nil)
    setPicture("proj.png")
    @count = 100
  end

  def play
    @count -= 1

    case direction
    when Joyau::Sprite::RIGHT
      move(3, 0)
    when Joyau::Sprite::LEFT
      move(-3, 0)
    when Joyau::Sprite::DOWN
      move(0, 3)
    when Joyau::Sprite::UP
      move(0, -3)
    end
  end

  attr_reader :count
  attr_accessor :has_collided
end
