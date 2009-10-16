require 'joyau/old'

initLib
initGfx

circle = Circle.new
circle.radius = 50
circle.setCenter(100, 100)

circle.color = color(255, 0, 0)

while mayPlay
  startDraw
  circle.draw
  endDraw

  sync
end

stopLib
stopGfx
