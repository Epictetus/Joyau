 initLib
 initGfx

 triangle = Triangle.new

 triangle.setPoints(10, 10, 100, 15, 150, 160)

 hash = Hash.new
 hash["r"] = 255
 hash["g"] = 0
 hash["b"] = 0
 hash["a"] = 255

 triangle.setColor(hash)

 while mayPlay
   startDraw
   triangle.draw
   endDraw

   sync
 end

 stopGfx
 stopLib
