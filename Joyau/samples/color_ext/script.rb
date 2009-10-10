require "joyau/color"

initLib
initGfx

text = DrawableText.new
text.color = Color.new(0, 0, 255)
text.background = Color.new(0, 0, 0, 0)
text.text = "Hello world!"

while mayPlay and Color.new(255, 255, 255) == Color.new(255, 255, 255, 255)
  startDraw
  text.draw
  endDraw

  sync
end

stopLib
stopGfx
