initLib
initGfx

text = DrawableText.new
text.setPos(50, 50)

text.color = color(255, 255, 255)
text.background = color(0, 0, 0, 0)
text.toggleStirring

text.setText "Hello world !"

while mayPlay
  startDraw
  clearScreen
  text.draw
  endDraw

  sync
end

stopLib
stopGfx
