initLib
initGfx

scroll = Scrolling.new
scroll.setSprite("bg.png")
scroll.setPos(0, 0)
scroll.setSpeed(3)
scroll.setDir($directions["LEFT"])

# I don't know why it works so.
# Juste a little way to avoid a graphic glitch, thus
for i in 0..480
  scroll.play
end

while mayPlay

  scroll.play

  startDraw
  scroll.draw
  endDraw

  sync
end

stopLib
stopGfx
