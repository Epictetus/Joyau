Joyau.initLib
Joyau.initGfx

sprite = Joyau::Sprite.new
sprite.picture = "sprite.png"
sprite.setAnim(3, 4)

sprite.autoDir = true

while Joyau.mayPlay
  Joyau::Pad.update

  sprite.move(0, -2) if Joyau::Pad.held? Joyau::Pad::UP
  sprite.move(0, 2) if Joyau::Pad.held? Joyau::Pad::DOWN
  sprite.move(-2, 0) if Joyau::Pad.held? Joyau::Pad::LEFT
  sprite.move(2, 0) if Joyau::Pad.held? Joyau::Pad::RIGHT
  
  Joyau.startDraw
  Joyau.clearScreen
  sprite.draw
  Joyau.endDraw

  Joyau.sync
end

Joyau.stopGfx
Joyau.stopLib
