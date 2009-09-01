initLib
initGfx

bg = Sprite.new("bg.png")

part = Particles.new
part.setFile("part.png")
part.setParam(50, 30, 4, 10)

pointerX = 0
pointerY = 0

while mayPlay
  Pad.update
  
  if Pad.held? Pad::CROSS
    part.addParticles(pointerX, pointerY)
  end

  if Pad.held? Pad::UP
    pointerY -= 1
  end
  if Pad.held? Pad::DOWN
    pointerY += 1
  end
  if Pad.held? Pad::RIGHT
    pointerX += 1
  end
  if Pad.held? Pad::DOWN
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
