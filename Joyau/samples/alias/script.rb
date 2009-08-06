initLib
initGfx

circle = Circle.new
circle.setCenter(240, 136)
circle.radius = 30

skip = false

while mayPlay
  if !skip
    startDraw
    clearScreen
    circle.draw
    drawText(0, 0, circle.x.to_s)
    drawText(0, 10, circle.y.to_s)
    drawText(0, 20, circle.w.to_s)
    drawText(0, 30, circle.h.to_s)
    endDraw
  end

  skip = sync
end

stopGfx
stopLib
