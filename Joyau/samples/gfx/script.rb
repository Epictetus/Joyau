initLib
initGfx

sprite = Sprite.new("image.png")
sprite.setPos(240 - sprite.w / 2, 136 - sprite.h / 2)

background = Sprite.new("bg.png")

while mayPlay
  Pad.update

  if Pad.held? Pad::RIGHT
    sprite.move(2, 0)
  end
  if Pad.held? Pad::LEFT
    sprite.move(-2, 0)
  end
  if Pad.held? Pad::UP
    sprite.move(0, -2)
  end
  if Pad.held? Pad::DOWN
    sprite.move(0, 2)
  end

  startDraw
  background.draw
  sprite.draw
  endDraw
  
  sync
end

stopGfx
stopLib

