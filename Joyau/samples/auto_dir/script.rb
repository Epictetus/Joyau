require 'joyau/old'

initLib
initGfx

sprite = Sprite.new
sprite.picture = "sprite.png"
sprite.setAnim(3, 4)

sprite.autoDir = true

while mayPlay
  Pad.update

  if Pad.held? Pad::UP
    sprite.move(0, -2);
  end
  if Pad.held? Pad::DOWN
    sprite.move(0, 2);
  end
  if Pad.held? Pad::LEFT
    sprite.move(-2, 0);
  end
  if Pad.held? Pad::RIGHT
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
