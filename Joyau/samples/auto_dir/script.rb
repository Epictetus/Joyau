initLib
initGfx

sprite = Sprite.new
sprite.setPicture "sprite.png"
sprite.setAnim(3, 4)

sprite.autoDir = true

while mayPlay
  readKeys
  if $keys["up"]
    sprite.move(0, -2);
  end
  if $keys["down"]
    sprite.move(0, 2);
  end
  if $keys["left"]
    sprite.move(-2, 0);
  end
  if $keys["right"]
    sprite.move(2, 0);
  end

  startDraw
  clearScreen
  sprite.draw
  endDraw

  sync
end

stopGfx
stopLib
