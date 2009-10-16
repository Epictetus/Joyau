require 'joyau/old'

initLib
initGfx

text = DrawableText.new
text.color = color(0, 0, 255)
text.background = color(0, 0, 0, 0)
text.text = "Hello world!"

while mayPlay
  startDraw
  text.draw
  endDraw

  sync
end

stopLib
stopGfx
