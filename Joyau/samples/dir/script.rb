initLib
initGfx

puts "would start"
dir = PSPDir.new
dir.setDir("ms0:/PSP/GAME/")
array = dir.ls

while mayPlay
  startDraw
  y = 0
  for var in array
    puts var.to_s
    #drawText(0, y, var.to_s)
    y += 20
  end
  endDraw

  sync
end

stopGfx
stopLib
