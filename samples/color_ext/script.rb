require 'joyau/old'
require "joyau/color"

initLib
initGfx

text = DrawableText.new
text.color = Joyau::Color.new(0, 0, 255)
text.background = Joyau::Color.new(0, 0, 0, 0)
text.text = "Hello world!"

while mayPlay
  startDraw
  text.draw
  endDraw

  sync
end

stopLib
stopGfx
