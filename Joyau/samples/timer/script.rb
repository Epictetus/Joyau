initLib
initGfx

timer = Timer.new

while mayPlay
  readKeys

  if $keys["pressed_cross"]
    timer.pause
  elsif $keys["pressed_square"]
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
