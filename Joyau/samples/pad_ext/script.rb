require "joyau/pad"

initLib
initGfx

while mayPlay
  Pad.update

  startDraw
  clearScreen
  drawText(0, 0, Pad.held_nbr.to_s)
  endDraw

  sync
end

stopLib
stopGfx
