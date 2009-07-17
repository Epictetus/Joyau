initLib
initGfx

text = "This is a file sample\n"

File.open("tmp", "w") do |file|
  file.puts text
end

text2 = File.read("tmp")

while mayPlay
  startDraw
  drawText(200, 0, text)
  drawText(200, 10, text2)
  endDraw

  sync
end

stopGfx
stopLib
