require 'joyau/old'

initLib
initGfx

rect = DrawableRect.new

rect.resize(100, 100)
rect.setPos(0, 0)

rect.color = color(255, 0, 0)

bRect = rect.boundingRect()

while mayPlay
  startDraw
  rect.draw
  drawText(50, 100, "x : " + bRect.x.to_s)
  drawText(50, 110, "y : " + bRect.y.to_s)
  drawText(50, 120, "w : " + bRect.w.to_s)
  drawText(50, 130, "h : " + bRect.h.to_s)
  endDraw

  sync
end

stopGfx
stopLib
