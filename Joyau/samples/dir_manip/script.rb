initLib
initGfx

text1 = "Cross : Create the Hello folder"
text2 = "Square : Remove it"

length = getLength(text1)
abs = 240 - length / 2

while mayPlay
  Pad.update

  if Pad.pressed? Pad::CROSS # Creating the same folders twice is weird
    mkdir "Hello"
  elsif Pad.pressed? Pad::SQUARE
    rmdir "Hello"
  end

  startDraw
  drawText(abs, 131, text1)
  drawText(abs, 141, text2)
  endDraw

  sync
end

stopGfx
stopLib
