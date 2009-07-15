initLib
initGfx

angel = Sprite.new
angel.setPicture("angel.png")
angel.setAnim(3, 4)

angel.setDirection($directions["DOWN"]) # Let's set a default direction

bg = Sprite.new
bg.setPicture("bg.png")

while mayPlay
  readKeys

  if $keys["up"]
    angel.move(0, -2)
    angel.setDirection($directions["UP"])
  end
  if $keys["down"]
    angel.move(0, 2)
    angel.setDirection($directions["DOWN"])
  end
  if $keys["left"]
    angel.move(-2, 0)
    angel.setDirection($directions["LEFT"])
  end
  if $keys["right"]
    angel.move(2, 0)
    angel.setDirection($directions["RIGHT"])
  end

  startDraw
  bg.draw
  angel.draw
  endDraw

  sync
end

stopGfx
stopLib
