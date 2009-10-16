require 'joyau/old'

initLib
initGfx

timer = Timer.new

while mayPlay
  Pad.update

  if Pad.pressed? Pad::CROSS
    timer.pause
  elsif Pad.pressed? Pad::SQUARE
    timer.resume
  end

  startDraw
  clearScreen
  drawText(0, 0, "Time : " + timer.getTime.to_s)
  endDraw

  sync
end

stopLib
stopGfx
