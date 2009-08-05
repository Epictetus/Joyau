initLib
initGfx

txt = ""

skip = false

while mayPlay
  readKeys
  if $keys["pressed_cross"]
    txt = gets
  end

  startDraw
  clearScreen
  drawText(0, 0, txt)
  endDraw

  sync
end

stopGfx
stopLib
