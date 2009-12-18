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

Joyau.draw do
  Joyau.clearScreen
  buf.draw
end

Joyau.screenshot "gray.png"

Joyau.stopGfx
Joyau.stopLib
