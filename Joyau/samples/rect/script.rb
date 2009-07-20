initLib
initGfx

rect = DrawableRect.new

rect.resize(100, 100)
rect.setPos(0, 0)

hash = color(255, 0, 0, 255)
rect.setColor(hash)

bRect = rect.boundingRect()

while mayPlay
  startDraw
  rect.draw
  drawText(50, 100, "x : " + bRect.getX.to_s)
  drawText(50, 110, "y : " + bRect.getY.to_s)
  drawText(50, 120, "w : " + bRect.getW.to_s)
  drawText(50, 130, "h : " + bRect.getH.to_s)
  endDraw

  sync
end

stopGfx
stopLib
