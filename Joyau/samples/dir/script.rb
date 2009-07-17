initLib
initGfx

array = Dir.entries("ms0:/PSP/GAME")

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
