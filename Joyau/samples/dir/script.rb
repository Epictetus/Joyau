initLib
initGfx

puts "creating a PSPDir object"
dir = PSPDir.new
puts "cd ms0:/PSP/GAME/"
dir.setDir("ms0:/PSP/GAME/")
puts "ls"
array = dir.ls

while mayPlay
  startDraw
  y = 0
  for var in array
    drawText(100, y, var.to_s)
    y += 20
  end
  endDraw

  sync
end

stopGfx
stopLib
