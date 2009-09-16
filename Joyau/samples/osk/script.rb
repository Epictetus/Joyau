initLib
initGfx

txt = ""

while mayPlay
  Pad.update
  if Pad.pressed? Pad::CROSS
    txt = gets
  end

  startDraw
  clearScreen
  drawScripted(0, 0, txt)
  endDraw

  sync
end

stopGfx
stopLib
