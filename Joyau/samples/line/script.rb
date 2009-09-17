initLib
initGfx

line = Line.new

line.setPos(0, 0)
line.setPoint(480, 272)

line.color = color(255, 0, 0)

rect = DrawableRect.new(0, 0, 5, 5)
rect.color = color(255, 255, 255)

while mayPlay
  Pad.update

  if Pad.held? Pad::UP
    rect.move(0, -2)
  end
  if Pad.held? Pad::DOWN
    rect.move(0, 2)
  end
  if Pad.held? Pad::RIGHT
    rect.move(2, 0)
  end
  if Pad.held? Pad::LEFT
    rect.move(-2, 0)
  end

  startDraw
  clearScreen
  line.draw
  rect.draw
  if line.collide(rect)
    drawText(0, 0, "ok")
  end
  endDraw

  sync
end

stopGfx
stopLib
