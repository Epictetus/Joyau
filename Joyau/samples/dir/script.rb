initLib
initGfx

dir = PSPDir.new
dir.setDir("ms0:/PSP/GAME/")
array = dir.ls

while mayPlay
  startDraw
  for var in array
    puts var.to_s
  end
  endDraw

  sync
end

stopGfx
stopLib
