require 'joyau/old'

initLib
initGfx

bg = Sprite.new("bg.png")

cursor = Cursor.new("cur.png", 40)
cursor.setPos(240 - cursor.w / 2, 136 - cursor.h / 2)
cursor.rect = Rect.new(50, 50, 100, 100)

obstacle = Sprite.new("obstacle.png")

setTextBackground color(0, 0, 0, 0)

skip = false

while mayPlay
  cursor.updatePos
  if obstacle.collide(cursor)
    cursor.cancelMove
  end

  if !skip
    startDraw
    
    bg.draw
    obstacle.draw
    cursor.draw

    drawText(0, 0, "x : " + cursor.x.to_s)
    drawText(0, 10, "y : " + cursor.y.to_s)

    endDraw
  end

  skip = sync
end

stopGfx
stopLib
