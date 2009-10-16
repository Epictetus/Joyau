require 'joyau/old'
require 'joyau/viewport'

initLib
initGfx

spr = Sprite.new("pic.png")
viewport = Joyau::Viewport.new

viewport << spr

while mayPlay
  Pad.update

  if Pad.held? Pad::LEFT
    viewport.move(6, 0)
  elsif Pad.held? Pad::RIGHT
    viewport.move(-6, 0)
  end

  spr.move(1, 0)

  startDraw
  clearScreen
  viewport.draw
  endDraw

  sync
end

stopGfx
stopLib
