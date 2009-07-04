initLib
initGfx

bg = Sprite.new
bg.setPicture("bg.png")

part = Particles.new
part.setFile("part.png")
part.setParam(50, 30, 4, 10)

pointerX = 0
pointerY = 0

while mayPlay
  readKeys
  
  if $keys["cross"]
    part.addParticles(pointerX, pointerY)
  end

  if $keys["up"]
    pointerY -= 1
  end
  if $keys["down"]
    pointerY += 1
  end
  if $keys["right"]
    pointerX += 1
  end
  if $keys["left"]
    pointerX -= 1
  end

  startDraw
  bg.draw
  part.draw
  endDraw

  sync
end

stopGfx
stopLib
