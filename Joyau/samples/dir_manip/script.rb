initLib
initGfx

text1 = "Cross : Create the Hello folder"
text2 = "Square : Remove it"

length1 = getLength(text1)
abs = 240 - length1/2

while mayPlay
  readKeys

  if $keys["pressed_cross"]#Creating the same folders two times would be weird
    mkdir("Hello")
  elsif $keys["pressed_square"]
    rmdir("Hello")
  end

  startDraw
  drawText(abs, 131, text1)
  drawText(abs, 141, text2)
  endDraw

  sync
end

stopGfx
stopLib
