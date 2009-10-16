require 'joyau/old'

initLib
initGfx

triangle = Triangle.new
triangle.setPoints(10, 10, 100, 15, 150, 160)
triangle.color = color(255, 0, 0)

while mayPlay
  startDraw
  triangle.draw
  endDraw
  
  sync
end

stopGfx
stopLib
