initLib
initGfx

line = Line.new

line.setPos(0, 0)
line.setPoint(480, 272)

line.color = color(255, 0, 0)

while mayPlay
  startDraw
  line.draw
  endDraw

  sync
end

stopGfx
stopLib
