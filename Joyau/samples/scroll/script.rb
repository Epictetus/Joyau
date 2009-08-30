initLib
initGfx

scroll = Scrolling.new("bg.png")
scroll.pos = Point.new(0, 0)
scroll.speed = 3
scroll.dir = Scrolling::LEFT

while mayPlay
  scroll.play

  startDraw
  scroll.draw
  endDraw

  sync
end

stopLib
stopGfx
