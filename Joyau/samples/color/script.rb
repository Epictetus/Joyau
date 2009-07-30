initLib
initGfx

text = DrawableText.new
text.setColor color(255, 255, 255)
text.setBackground color(0, 0, 0, 0)
text.setText "Hello world!"

while mayPlay
  startDraw
  text.draw
  endDraw

  sync
end

stopLib
stopGfx
