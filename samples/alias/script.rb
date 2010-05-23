require 'joyau/color'

Joyau.initLib
Joyau.initGfx

circle = Joyau::Circle.new
circle.center = Joyau::Point.new(240, 136)
circle.color = Joyau::Color::WHITE
circle.radius = 30

skip = false

while Joyau.mayPlay
  if !skip
    Joyau.startDraw
    Joyau.clearScreen
    circle.draw
    Joyau.drawText(0, 0, circle.x.to_s)
    Joyau.drawText(0, 10, circle.y.to_s)
    Joyau.drawText(0, 20, circle.w.to_s)
    Joyau.drawText(0, 30, circle.h.to_s)
    Joyau.endDraw
  end

  skip = Joyau.sync
end

Joyau.stopGfx
Joyau.stopLib
