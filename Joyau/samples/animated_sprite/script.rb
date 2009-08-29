initLib
initGfx

angel = Sprite.new("angel.png")
angel.setAnim(3, 4)

angel.direction = Sprite::DOWN

bg = Sprite.new("bg.png")

while mayPlay
  readKeys

  if $keys["up"]
    angel.move(0, -2)
    angel.direction = Sprite::UP
  end
  if $keys["down"]
    angel.move(0, 2)
    angel.setDirection($directions["DOWN"]) # this works too
  end
  if $keys["left"]
    angel.move(-2, 0)
    angel.direction = Sprite::LEFT # as well as this
  end
  if $keys["right"]
    angel.move(2, 0)
    angel.direction = $directions["RIGHT"] # and that
  end

  startDraw
  bg.draw
  angel.draw
  endDraw

  sync
end

stopGfx
stopLib
