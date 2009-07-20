initLib
initGfx

circle = Circle.new
circle.setRadius(50)
circle.setCenter(100, 100)

hash = Hash.new
hash["r"] = 255
hash["g"] = 0
hash["b"] = 0
hash["a"] = 255

circle.setColor(hash)

while mayPlay
  startDraw
  circle.draw
  endDraw

  sync
end

stopLib
stopGfx
