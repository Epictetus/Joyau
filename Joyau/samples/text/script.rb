initLib
initGfx

text = DrawableText.new
text.color = color(255, 0, 0)
text.background = color(0, 0, 255)

text.text = "Hello world !"

text.setPos(100, 100)

while mayPlay
  startDraw
  text.draw
  endDraw

  sync
end

stopGfx
stopLib
