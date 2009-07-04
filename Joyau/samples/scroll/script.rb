initLib
initGfx

scroll = Scrolling.new
scroll.setSprite("bg.png")
scroll.setPos(0, 0)
scroll.setSpeed(3)
scroll.setDir($directions["LEFT"])

while mayPlay

  scroll.play

  startDraw
  scroll.draw
  endDraw

  sync
end

stopLib
stopGfx
