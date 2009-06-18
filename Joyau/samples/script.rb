initLib
initGfx

sprite = Sprite.new
sprite.setPicture("image.png")
w = sprite.getW
h = sprite.getH
sprite.setPos(480 / 2 - w / 2, 272 / 2 - h / 2)

while mayPlay
  readKeys

  if $keys["right"]
    sprite.move(2, 0)
  end
  if $keys["left"]
    sprite.move(-2, 0)
  end
  if $keys["up"]
    sprite.move(0, -2)
  end
  if $keys["down"]
    sprite.move(0, 2)
  end

  startDraw
  clearScreen
  sprite.draw
  endDraw
  
  audioSync
  sync
end

stopGfx
stopLib

