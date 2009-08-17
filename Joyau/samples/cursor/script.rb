initLib
initGfx

bg = Sprite.new("bg.png")

cursor = Cursor.new("cur.png", 40)
cursor.setPos(240 - cursor.getW / 2, 136 - cursor.getH / 2)

setTextBackground color(0, 0, 0, 0)

skip = false

while mayPlay
  cursor.updatePos

  if !skip
    startDraw
    
    bg.draw
    cursor.draw

    drawText(0, 0, "x : " + cursor.getX.to_s)
    drawText(0, 10, "y : " + cursor.getY.to_s)

    endDraw
  end

  skip = sync
end

stopGfx
stopLib
