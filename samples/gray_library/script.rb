require 'joyau/enumerable'

module GrayLibrary
  @@buffers = {}
  class << self
    def [](filename)
      if @@buffers[filename].nil?
        create_buffer(filename)
      else
        @@buffers[filename]
      end
    end

    def clear_buffers
      @@buffers.clear
    end

    private
    def create_buffer(filename)
      @@buffers[filename] = Joyau::Buffer[filename]
      
       @@buffers[filename].lock do
        @@buffers[filename].each_with_pos do |x, y, col|
          gray = (0.299 * col.r + 0.587 * col.g + 0.114 * col.b).to_i
          @@buffers[filename][x, y] = Joyau::Color.new(gray, gray, gray, col.a)
        end
      end

      return @@buffers[filename]
    end
  end
end

class GraySprite < Joyau::Sprite
  class << self
    def new(filename = nil)
      obj = nil
      unless filename
        obj = super()
      else
        obj = GrayLibrary[filename].to_sprite
        obj.res_name = "gray:#{filename}"
      end
      
      return obj
    end
  end
end

Joyau.initLib
Joyau.initGfx

bg = GraySprite.new("bg.png")

sprite = GraySprite.new("angel.png")
sprite.setAnim(3, 4)
sprite.autoDir = true

sprite2 = GraySprite.new("angel.png")
sprite2.setAnim(3, 4)
sprite2.dir = Joyau::Sprite::DOWN
sprite2.setPos(100, 100)

while Joyau.mayPlay
  Joyau::Pad.update

  sprite.move(2, 0)  if Joyau::Pad.held? Joyau::Pad::RIGHT
  sprite.move(-2, 0) if Joyau::Pad.held? Joyau::Pad::LEFT
  sprite.move(0, -2) if Joyau::Pad.held? Joyau::Pad::UP
  sprite.move(0, 2)  if Joyau::Pad.held? Joyau::Pad::DOWN

  Joyau.draw do
    bg.draw
    sprite.draw
    sprite2.draw
  end
end

Joyau.stopGfx
Joyau.stopLib
