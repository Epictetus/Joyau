initLib
initGfx

sprite = Sprite.new
sprite.setPicture "sprite.png"

sprite.setPos(240 - sprite.getW / 2, 136 - sprite.getH / 2)

obstacle = Sprite.new
obstacle.setPicture "obstacle.png"

skip = false

while mayPlay
  readKeys
  if $keys["up"]
    sprite.move(0, -2)
  elsif $keys["down"]
    sprite.move(0, 2)
  elsif $keys["right"]
    sprite.move(2, 0)
  elsif $keys["left"]
    sprite.move(-2, 0)
  end

  if obstacle.collide(sprite)
    sprite.cancelMove
  end

  if !skip
    startDraw
    clearScreen
    obstacle.draw
    sprite.draw
    endDraw
  end

  skip = sync
end

stopGfx
stopLib
