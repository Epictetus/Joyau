initLib
initGfx

line = Line.new

line.setPos(0, 0)
line.setPoint(480, 272)

hash = Hash.new
hash["r"] = 255
hash["g"] = 0
hash["b"] = 0
hash["a"] = 255

line.setColor(hash)

while mayPlay
  startDraw
  line.draw
  endDraw

  sync
end

stopGfx
stopLib
