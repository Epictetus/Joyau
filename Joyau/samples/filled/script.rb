initLib
initGfx

circle = Circle.new
circle.radius = 50
circle.setCenter(100, 100)
circle.filled = false

while mayPlay
  startDraw
  circle.draw
  endDraw

  sync
end

stopLib
stopGfx
