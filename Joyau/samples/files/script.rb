initLib
initGfx

text = "This is a file sample\n"
puts text
file = PSPFile.new
file.open("tmp")

puts "we'll write into the file"
file.write(text)
puts "everything has been written"

file.close

file2 = PSPFile.new
file2.open("tmp")

puts "reading the file"
text2 = file2.getLine

while mayPlay
  startDraw
  drawText(200, 0, text)
  drawText(200, 10, text2)
  endDraw

  sync
end

stopGfx
stopLib
