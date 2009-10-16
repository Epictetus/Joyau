require 'joyau/old'
require 'joyau/cursor'

class AnalogicSprite < Joyau::Sprite
  include Joyau::StickFollower
end

initLib
initGfx

bg = Sprite.new("bg.png")

cursor = AnalogicSprite.new("cur.png")
cursor.setPos(240 - cursor.w / 2, 136 - cursor.h / 2)
cursor.cursor_conf(40, Rect.new(0, 0, 100, 100))

setTextBackground color(0, 0, 0, 0)

skip = false

while mayPlay
  cursor.update_pos

  if !skip
    startDraw
    
    bg.draw
    cursor.draw

    drawText(0,  0, "x : " + cursor.x.to_s)
    drawText(0, 10, "y : " + cursor.y.to_s)
 
    endDraw
  end

  skip = sync
end

stopGfx
stopLib
