initLib
initGfx

sprite = Sprite.new("sprite.png")
sprite.pos = Point.new(240 - sprite.w / 2, 136 - sprite.h / 2)

obstacle = Sprite.new("obstacle.png")

skip = false

while mayPlay
  Pad.update

  if Pad.held? Pad::UP
    sprite.move(0, -2)
  elsif Pad.held? Pad::DOWN
    sprite.move(0, 2)
  elsif Pad.held? Pad::RIGHT
    sprite.move(2, 0)
  elsif Pad.held? Pad::LEFT
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
