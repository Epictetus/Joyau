require 'joyau/viewport'

Joyau.initLib
Joyau.initGfx

spr = Joyau::Sprite.new("pic.png")
viewport = Joyau::Viewport.new

viewport << spr

while Joyau.mayPlay
  Joyau::Pad.update

  if Joyau::Pad.held? Joyau::Pad::LEFT
    viewport.move(6, 0)
  elsif Joyau::Pad.held? Joyau::Pad::RIGHT
    viewport.move(-6, 0)
  end

  spr.move(1, 0)

  Joyau.startDraw
  Joyau.clearScreen
  viewport.draw
  Joyau.endDraw

  Joyau.sync
end

Joyau.stopGfx
Joyau.stopLib
