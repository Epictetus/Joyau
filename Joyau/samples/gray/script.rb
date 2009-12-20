require 'joyau/color'

Joyau.initLib
Joyau.initGfx

sprite = Joyau::Sprite.new("angel.png")

until Joyau::Pad.pressed? Joyau::Pad::CROSS
  Joyau::Pad.update
  Joyau.draw do
    sprite.draw
  end
end

buf = sprite.to_buf

buf.lock do
  (0..(buf.w-1)).each do |x|
    (0..(buf.h-1)).each do |y|
      col = buf[x, y].to_col
            
      gray = (0.299 * col.r + 0.587 * col.g + 0.114 * col.b).to_i
      
      col.r = gray
      col.g = gray
      col.b = gray
    
      buf[x, y] = col
    end
  end
end

Joyau::Pad.update
Joyau.draw do
  buf.draw
end

sprite = buf.to_sprite
sprite.setAnim(3, 4)
sprite.autoDir = true

while Joyau.mayPlay
  Joyau::Pad.update

  sprite.move(2, 0)  if Joyau::Pad.held? Joyau::Pad::RIGHT
  sprite.move(-2, 0) if Joyau::Pad.held? Joyau::Pad::LEFT
  sprite.move(0, -2) if Joyau::Pad.held? Joyau::Pad::UP
  sprite.move(0, 2)  if Joyau::Pad.held? Joyau::Pad::DOWN
  
  Joyau.draw do
    Joyau.clearScreen
    sprite.draw
  end
end

Joyau.stopGfx
Joyau.stopLib
